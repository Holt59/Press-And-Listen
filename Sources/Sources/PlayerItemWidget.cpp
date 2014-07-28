#include "PlayerItemWidget.h"

#include <QStyle>

PlayerItemWidget::PlayerItemWidget (PressAndListenPlayer *player, QWidget *parent) : QLabel(parent), m_player (player) {
    this->setWordWrap (true) ;
    connect (player, &PressAndListenPlayer::update, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songChanged, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songPaused, this, &PlayerItemWidget::updateInformation);
    connect (player, &PressAndListenPlayer::songResume, this, &PlayerItemWidget::updateInformation);
    setStyleSheet (" \
        PlayerItemWidget { padding: 10px ; } \
        PlayerItemWidget[current-player=true] { padding-left: 4px ; border-left-width: 6px; border-style: solid ; border-color: #88F; } \
        PlayerItemWidget:disabled { background: #eee ; } ") ; 
    updateInformation () ;
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
    setText (PlayerInfo::toString (m_player->player ()) + " on " + PlayerInfo::toString (m_player->browser ())
             + QString ("<br />")
             + QString ("<span class=\"song-info\">") + songInfo + QString ("</span>"));
    this->setEnabled (m_player->isEnable ());
    this->setProperty ("current-player", m_player->isCurrentPlayer ());
    this->style ()->unpolish (this) ;
    this->style ()->polish (this) ;
}