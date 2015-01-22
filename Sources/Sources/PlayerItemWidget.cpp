#include "PlayerItemWidget.h"

#include <QStyle>
#include <QHBoxLayout>
#include <QPixmap>

PlayerItemWidget::PlayerItemWidget (PressAndListenPlayer *player, QWidget *parent) : QFrame(parent), m_player (player) {

    this->setFixedHeight (50) ;

    QHBoxLayout *layout = new QHBoxLayout () ;
    this->setLayout (layout) ;

    m_active = new QFrame () ;
    m_active->setFixedWidth (5) ;

    m_logo = new QLabel () ;
    QPixmap logo (PlayerInfo::toString (player->player ()).toLower () + QString (".png")) ;
    logo = logo.scaled (30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation) ;
    m_logo->setPixmap (logo) ;

    m_text = new QLabel () ;
    m_text->setWordWrap (true) ;

    layout->addWidget (m_active) ;
    layout->addWidget (m_logo, 0) ;
    layout->addSpacing (5) ;
    layout->addWidget (m_text, 1) ;

    updateInformation () ;

    connect (player, &PressAndListenPlayer::update, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songChanged, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songPaused, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songResume, this, &PlayerItemWidget::updateInformation);
}

void PlayerItemWidget::updateInformation () {
    QString songInfo ;
    if (m_player->currentSong ().isValid ()) {
        songInfo = m_player->currentSong ().playing () ? QString ("Playing") : QString ("Paused") ;
        songInfo += QString (" - ") ;
        songInfo += m_player->currentSong ().title () ;
        songInfo += QString (" (") + m_player->currentSong ().artist () + QString (")") ;
    }
    else {
        songInfo = QString ("No song playing.");
    }
    m_text->setText (QString ("<span class=\"song-info\">") + songInfo + QString ("</span>"));
    this->setEnabled (m_player->isEnable ());
    if (m_player->isCurrentPlayer ()) {
        m_active->setStyleSheet ("QFrame { background: blue ; }") ;
    }
    else {
        m_active->setStyleSheet ("QFrame { background: transparent ; }") ;
    }
}