#ifndef _PLAYLISTWIDGET
#define _PLAYLISTWIDGET

#include <QLabel>
#include <QTableWidget>
#include <QAction>

#include "PlayerItemWidget.h"
#include "PressAndListenPlayer.h"

class PlayerListWidget : public QTableWidget {

    Q_OBJECT

public:
    PlayerListWidget (QWidget *parent = 0);
    ~PlayerListWidget ();
    
Q_SIGNALS:

    void playerSleep (PressAndListenPlayer *) ;
    void playerWakeUp (PressAndListenPlayer *) ;
    void playerSetAsCurrent (PressAndListenPlayer *) ;
    void playerFocusTab (PressAndListenPlayer *) ;
    void playerCloseTab (PressAndListenPlayer *) ;

public Q_SLOTS:

    void onPlayerEnter (PressAndListenPlayer *) ;
    void onPlayerLeave (PressAndListenPlayer *) ;
    
private Q_SLOTS:

    void onCustomMenuRequested (QPoint) ;

    void onUserPlayerSleep () ;
    void onUserPlayerWakeUp () ;
    void onUserPlayerSetAsCurrent () ;
    void onUserPlayerFocusTab () ;
    void onUserPlayerCloseTab () ;

private:

    int getRowForPlayer (PressAndListenPlayer *player);
    PlayerItemWidget *getCellWidget (int row, int column = 0) ;
    PlayerItemWidget *getCellWidget (PressAndListenPlayer *player) ;

    void createActions () ;

    QAction *m_closeTabPlayer, *m_focusTabPlayer, *m_setAsCurrentPlayer, *m_sleepPlayer, *m_wakeUpPlayer ;
    QList <PressAndListenPlayer *> m_players ;
    int m_contextMenuRow ;
};

#endif

