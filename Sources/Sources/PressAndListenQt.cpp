#include "PressAndListenQt.h"

#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QHBoxLayout>

typedef PressAndListenSettings::NotificationType NotifType ;
typedef PressAndListenSettings PSettings ;

PressAndListenQt::PressAndListenQt (QWidget *parent) : 
QMainWindow (parent), m_server (new PressAndListenServer (52132, parent)), m_hotKeyEvent(true) {

    PlayerListWidget *pwidget = new PlayerListWidget (this) ;
    setCentralWidget (pwidget);

    setWindowIcon (QIcon ("icon128.png")) ;
    setWindowTitle ("Press & Listen") ;

    m_notifDialog = new NotificationSettingsDialog (this) ;

    createActions () ;
    createMenus () ;
    createStatusBar () ;
    createTrayIcon () ;

    m_trayIcon->show () ;

    connect (m_server, &PressAndListenServer::playerEnter, this, &PressAndListenQt::onPlayerEnter);
    connect (m_server, &PressAndListenServer::playerLeave, this, &PressAndListenQt::onPlayerLeave); 

    connect (m_server, &PressAndListenServer::playerEnter, pwidget, &PlayerListWidget::onPlayerEnter);
    connect (m_server, &PressAndListenServer::playerLeave, pwidget, &PlayerListWidget::onPlayerLeave);
    
    connect (pwidget, &PlayerListWidget::playerSleep, m_server, &PressAndListenServer::onUserSleepPlayer);
    connect (pwidget, &PlayerListWidget::playerWakeUp, m_server, &PressAndListenServer::onUserWakeUpPlayer);
    connect (pwidget, &PlayerListWidget::playerSetAsCurrent, this, &PressAndListenQt::onPlayerSetAsCurrent);
    connect (pwidget, &PlayerListWidget::playerFocusTab, this, &PressAndListenQt::onPlayerFocusTab);
    connect (pwidget, &PlayerListWidget::playerCloseTab, this, &PressAndListenQt::onPlayerCloseTab);

}

PressAndListenQt::~PressAndListenQt () { 
    m_trayIcon->hide () ;
    m_trayIcon->deleteLater () ;
    m_server->close () ;
    m_server->deleteLater () ; 
}

void PressAndListenQt::showAboutPopup () {
    QMessageBox::about (this, tr ("About"), tr("<p>Press & Listen, <br />© 2014 Mikaël Capelle</p>")) ;
}

void PressAndListenQt::createActions () {
    m_minimizeAction = new QAction (tr ("Mi&nimize"), this);
    connect (m_minimizeAction, &QAction::triggered, this, &QWidget::hide);

    m_restoreAction = new QAction (tr ("&Restore"), this);
    connect (m_restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    m_quitAction = new QAction (tr ("&Quit"), this);
    connect (m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    m_aboutAction = new QAction (tr ("&About"), this);
    connect (m_aboutAction, &QAction::triggered, this, &PressAndListenQt::showAboutPopup);

    m_notifSettingsAction = new QAction (tr ("&Notification Settings"), this);
    connect (m_notifSettingsAction, &QAction::triggered, m_notifDialog, &NotificationSettingsDialog::show);
}

void PressAndListenQt::createMenus () {
    QMenu *filemenu = menuBar ()->addMenu (tr ("&File")) ;
    filemenu->addAction (m_minimizeAction);
    filemenu->addSeparator ();
    filemenu->addAction (m_quitAction);

    QMenu *settingsmenu = menuBar ()->addMenu (tr ("&Settings")) ;
    settingsmenu->addAction (m_notifSettingsAction);

    menuBar ()->addAction (m_aboutAction) ;
}

void PressAndListenQt::createStatusBar () {
    QStatusBar *bar = this->statusBar () ;
    updateStatusBar () ;
}

void PressAndListenQt::updateStatusBar () {
    if (m_server->currentPlayer () == nullptr) {
        statusBar ()->showMessage (tr ("No player connected.")) ;
    }
    else {
        PressAndListenPlayer *player = m_server->currentPlayer () ;
        PressAndListenSong song = player->currentSong () ;
        statusBar ()->showMessage ((song.playing() ? tr("Playing") : tr("Paused")) 
                                   + QString (" [") + PlayerInfo::toString (player->player()) + QString (", ") + PlayerInfo::toString (player->browser()) + QString ("]")
                                   + QString (": ") + song.title() + QString(" - ") + song.artist()) ;
    }
}

void PressAndListenQt::createTrayIcon () {
    QMenu *trayIconMenu = new QMenu (this);
    trayIconMenu->addAction (m_minimizeAction);
    trayIconMenu->addAction (m_restoreAction);
    trayIconMenu->addSeparator ();
    trayIconMenu->addAction (m_quitAction);

    m_trayIcon = new QSystemTrayIcon (this);
    m_trayIcon->setIcon (QIcon ("icon128.png")) ;
    m_trayIcon->setContextMenu (trayIconMenu);
    m_trayIcon->setToolTip ("Press & Listen");
}

void PressAndListenQt::onPlayerEnter (PressAndListenPlayer * player) {
    if (PSettings ().showNotification (NotifType::PlayerEnter)) {
        m_trayIcon->showMessage ("Press & Listen", QString ("New player: ") + PlayerInfo::toString (player->player ())) ;
    }
    connect (player, &PressAndListenPlayer::songChanged, this, &PressAndListenQt::onSongChanged);
    connect (player, &PressAndListenPlayer::songPaused, this, &PressAndListenQt::onSongPaused);
    connect (player, &PressAndListenPlayer::songResume, this, &PressAndListenQt::onSongResume);
    updateStatusBar () ;
}

void PressAndListenQt::onPlayerLeave (PressAndListenPlayer * player) {
    if (PSettings ().showNotification (NotifType::PlayerLeave)) {
        m_trayIcon->showMessage ("Press & Listen", QString ("End player: ") + PlayerInfo::toString (player->player ())) ;
    }
    updateStatusBar () ;
}

void PressAndListenQt::onPlayerSetAsCurrent (PressAndListenPlayer * player) {
    if (m_server->currentPlayer () == player) {
        return ;
    }
    if (m_server->currentPlayer ()) {
        m_server->currentPlayer ()->pause () ;
    }
    m_server->setCurrentPlayer (player)->play () ;
    updateStatusBar () ;
}

void PressAndListenQt::onPlayerFocusTab (PressAndListenPlayer * player) {
    player->focusTab () ;
}

void PressAndListenQt::onPlayerCloseTab (PressAndListenPlayer * player) {
    player->closeTab () ;
}

void PressAndListenQt::onSongChanged (PressAndListenSong const& oldSong, PressAndListenSong const& currentSong) {
    if (!PSettings ().showNotification (NotifType::PlayerSwitchSong)) {
        return ;
    }
    PressAndListenPlayer *player = qobject_cast <PressAndListenPlayer *> (sender ());
    if (oldSong.isValid () || currentSong.playing ()) {
        m_trayIcon->showMessage ("Press & Listen", (currentSong.playing () ? QString ("Playing [") : QString ("Paused [")) + PlayerInfo::toString (player->player ()) + ("]: ")
                                 + currentSong.title () + QString (" (") + currentSong.artist () + QString (")")) ;
    }
    updateStatusBar () ;
}
void PressAndListenQt::onSongPaused (PressAndListenSong const& currentSong) {
    if (!PSettings ().showNotification (NotifType::PlayerResume)) {
        return ;
    }
    PressAndListenPlayer *player = qobject_cast <PressAndListenPlayer *> (sender ());
    m_trayIcon->showMessage ("Press & Listen", QString ("Paused [") + PlayerInfo::toString (player->player ()) + ("]: ")
                             + currentSong.title () + QString (" (") + currentSong.artist () + QString (")")) ;
    updateStatusBar () ;
}

void PressAndListenQt::onSongResume (PressAndListenSong const& currentSong) {
    if (!PSettings ().showNotification (NotifType::PlayerPaused)) {
        return ;
    }
    PressAndListenPlayer *player = qobject_cast <PressAndListenPlayer *> (sender ());
    m_trayIcon->showMessage ("Press & Listen", QString ("Playing [") + PlayerInfo::toString (player->player ()) + ("]: ")
                             + currentSong.title () + QString (" (") + currentSong.artist () + QString (")")) ;
    updateStatusBar () ;
}

void PressAndListenQt::changeEvent (QEvent * e) {
    if (e->type () == QEvent::WindowStateChange && this->isMinimized ()) {
        this->hide () ;
        e->ignore () ;
    }
}

bool PressAndListenQt::event (QEvent * e) {

    if (e->type () == MediaKeyListener::HotKey::AltStop) {
        m_hotKeyEvent = !m_hotKeyEvent ;
        return true ;
    }

    if (!m_hotKeyEvent || !m_server || m_server->currentPlayer () == nullptr) {
        return QWidget::event (e) ;
    }

    PressAndListenPlayer *player = m_server->currentPlayer () ;

    switch (e->type ()) {
        case MediaKeyListener::HotKey::Stop:
            break ;
        case MediaKeyListener::HotKey::Play: player->toggle () ;
            break ;
        case MediaKeyListener::HotKey::Prev: player->prev () ;
            break ;
        case MediaKeyListener::HotKey::Next: player->next () ;
            break ;
        case MediaKeyListener::HotKey::AltStop:
            m_hotKeyEvent = !m_hotKeyEvent ;
            break ;
        case MediaKeyListener::HotKey::AltPlay:
            break ;
        case MediaKeyListener::HotKey::AltPrev:
        {
            PressAndListenPlayer *oldPlayer = player ;
            player = m_server->prevPlayer () ;
            if (oldPlayer != player) {
                oldPlayer->pause () ;
                player->play () ;
            }
        }
            break ;
        case MediaKeyListener::HotKey::AltNext:
        {
            PressAndListenPlayer *oldPlayer = player ;
            player = m_server->prevPlayer () ;
            if (oldPlayer != player) {
                oldPlayer->pause () ;
                player->play () ;
            }
        }
            break ;
        default:
            return QWidget::event (e) ;
    }
    
    return true ;
}
