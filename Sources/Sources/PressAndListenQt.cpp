#include "PressAndListenQt.h"

#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QHBoxLayout>

#include "GlobalShortcut.h"

typedef PressAndListenSettings::NotificationType NotificationType ;
typedef PressAndListenSettings::ShortcutType ShortcutType ;

PressAndListenQt::PressAndListenQt (QWidget *parent) : 
QMainWindow (parent), m_server (new PressAndListenServer (52132, parent)){

    PlayerListWidget *pwidget = new PlayerListWidget (this) ;
    setCentralWidget (pwidget);

    setWindowIcon (QIcon ("icon128.png")) ;
    setWindowTitle ("Press & Listen") ;

    m_notifDialog = new NotificationSettingsDialog (this) ;

    createActions () ;
    createMenus () ;
    createStatusBar () ;
    createTrayIcon () ;

    try {
        createShortcuts ();
    }
    catch (GlobalShortcut::KeyNotBoundException const& kne) {
        qDebug () << kne.info () << " - " << kne.key () ;
        QMessageBox::critical (this, "Error binding keyboard keys", kne.info()) ;
        exit (EXIT_FAILURE) ; // Not qApp->exit() since the event loop is not running
    }

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
    QMessageBox::about (this, tr ("About"), tr("<h3>Press & Listen</h3> <p>© 2014<br />Mikaël Capelle<br />Jean-Baptiste Cayrou</p>")) ;
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

void PressAndListenQt::createShortcuts () {

    QMap <ShortcutType, void (PressAndListenQt::*) ()> shortcutSlots {
            {PressAndListenSettings::ShortcutType::Toggle, &PressAndListenQt::toggle},
            {PressAndListenSettings::ShortcutType::Next, &PressAndListenQt::next},
            {PressAndListenSettings::ShortcutType::Previous, &PressAndListenQt::prev},
            {PressAndListenSettings::ShortcutType::NextPlayer, &PressAndListenQt::nextPlayer},
            {PressAndListenSettings::ShortcutType::PreviousPlayer, &PressAndListenQt::prevPlayer}
    };

    for (auto shortcut : SETTINGS.getShortcutTypes ()) {
        for (auto key : SETTINGS.getShortcut (shortcut)) {
            connect (new GlobalShortcut (key), &GlobalShortcut::triggered, this, shortcutSlots[shortcut]) ;
        }
    }

}

void PressAndListenQt::onPlayerEnter (PressAndListenPlayer * player) {
    if (SETTINGS.showNotification (NotificationType::PlayerEnter)) {
        m_trayIcon->showMessage ("Press & Listen", QString ("New player: ") + PlayerInfo::toString (player->player ())) ;
    }
    connect (player, &PressAndListenPlayer::songChanged, this, &PressAndListenQt::onSongChanged);
    connect (player, &PressAndListenPlayer::songPaused, this, &PressAndListenQt::onSongPaused);
    connect (player, &PressAndListenPlayer::songResume, this, &PressAndListenQt::onSongResume);
    updateStatusBar () ;
}

void PressAndListenQt::onPlayerLeave (PressAndListenPlayer * player) {
    if (SETTINGS.showNotification (NotificationType::PlayerLeave)) {
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
    if (!SETTINGS.showNotification (NotificationType::PlayerSwitchSong)) {
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
    if (!SETTINGS.showNotification (NotificationType::PlayerResume)) {
        return ;
    }
    PressAndListenPlayer *player = qobject_cast <PressAndListenPlayer *> (sender ());
    m_trayIcon->showMessage ("Press & Listen", QString ("Paused [") + PlayerInfo::toString (player->player ()) + ("]: ")
                             + currentSong.title () + QString (" (") + currentSong.artist () + QString (")")) ;
    updateStatusBar () ;
}

void PressAndListenQt::onSongResume (PressAndListenSong const& currentSong) {
    if (!SETTINGS.showNotification (NotificationType::PlayerPaused)) {
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

void PressAndListenQt::pause () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    player->pause ();
}

void PressAndListenQt::play () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    player->play ();
}
void PressAndListenQt::toggle () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;

    player->toggle ();
}

void PressAndListenQt::prev () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    player->prev ();
}
void PressAndListenQt::next () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    player->next ();
}
void PressAndListenQt::prevPlayer () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    PressAndListenPlayer *oldPlayer = player ;
    player = m_server->prevPlayer () ;
    if (oldPlayer != player) {
        oldPlayer->pause () ;
        player->play () ;
    }
}
void PressAndListenQt::nextPlayer () {
    PressAndListenPlayer *player = m_server->currentPlayer () ;
    if (!player)
        return;
    PressAndListenPlayer *oldPlayer = player ;
    player = m_server->nextPlayer () ;
    if (oldPlayer != player) {
        oldPlayer->pause () ;
        player->play () ;
    }
}
