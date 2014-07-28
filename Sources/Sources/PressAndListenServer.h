#ifndef _PRESSANDLISTEN_SERVER_H
#define _PRESSANDLISTEN_SERVER_H

#include <QtWebSockets/QWebSocketServer>
#include <QException>

#include "PressAndListenPlayer.h"

class ServerNotStartedException : public QException {
public:
    ServerNotStartedException () : QException () {} 
    virtual ~ServerNotStartedException () throw () { }
};

class PressAndListenServer : public QWebSocketServer {

    Q_OBJECT

public:

    PressAndListenServer (quint16 port, QObject *parent = 0);
    virtual ~PressAndListenServer ();

    PressAndListenPlayer *currentPlayer () const { return m_players.isEmpty () ? nullptr : m_players[m_currentPlayer] ; }
    
Q_SIGNALS:

    void playerEnter (PressAndListenPlayer *) ;
    void playerLeave (PressAndListenPlayer *) ;

public Q_SLOTS:

    PressAndListenPlayer* prevPlayer () ;
    PressAndListenPlayer* nextPlayer () ;
    PressAndListenPlayer *setCurrentPlayer (PressAndListenPlayer*) ;

    void onUserSleepPlayer (PressAndListenPlayer *) ;
    void onUserWakeUpPlayer (PressAndListenPlayer *) ;

private Q_SLOTS :

    void onNewConnection () ;
    void onSongChanged (PressAndListenSong const&, PressAndListenSong const&);
    void onClose () ;

private:

    size_t m_currentPlayer ;
    QList <PressAndListenPlayer *> m_players ;
    QList <PressAndListenPlayer *> m_sleepingPlayers ;

};

#endif