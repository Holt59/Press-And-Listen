#ifndef PRESSANDLISTENQT_H
#define PRESSANDLISTENQT_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>

#include "PressAndListenServer.h"
#include "MediaKeyListener.h"
#include "PlayerListWidget.h"
#include "NotificationSettingsDialog.h"

class PressAndListenQt : public QMainWindow {

    Q_OBJECT

public:
    PressAndListenQt(QWidget *parent = 0);
    ~PressAndListenQt();

    virtual bool event (QEvent * e) Q_DECL_OVERRIDE ;

protected:
    virtual void changeEvent (QEvent * e) Q_DECL_OVERRIDE ;
    
private Q_SLOTS:

    void onPlayerEnter (PressAndListenPlayer *) ;
    void onPlayerLeave (PressAndListenPlayer *) ;

    void onPlayerSetAsCurrent (PressAndListenPlayer *) ;

    void onPlayerFocusTab (PressAndListenPlayer *) ;
    void onPlayerCloseTab (PressAndListenPlayer *) ;

    void onSongChanged (PressAndListenSong const&, PressAndListenSong const&);
    void onSongPaused (PressAndListenSong const&) ;
    void onSongResume (PressAndListenSong const&) ;

    void showAboutPopup () ;
    void updateStatusBar () ;

private:
    
    void createActions () ;
    void createTrayIcon () ;
    void createMenus () ;
    void createStatusBar () ;
    
    PressAndListenServer *m_server;
    bool m_hotKeyEvent ;
    QAction *m_minimizeAction, *m_restoreAction, *m_quitAction, *m_aboutAction, *m_notifSettingsAction ;
    QSystemTrayIcon *m_trayIcon ;
    NotificationSettingsDialog *m_notifDialog ;

};

#endif // PRESSANDLISTENQT_H
