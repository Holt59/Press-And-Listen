#ifndef _PRESSANDLISTEN_SERVER_H
#define _PRESSANDLISTEN_SERVER_H

#include <QtWebSockets/QWebSocketServer>
#include <QException>

#include "PressAndListenPlayer.h"

class ServerNotStartedException : public QException {
public:
    ServerNotStartedException (QWebSocketProtocol::CloseCode err, QString const& info) : QException (), _err (err), _info (info) {}
    virtual ~ServerNotStartedException () throw () { }

    QWebSocketProtocol::CloseCode error () const { return _err ; }
    QString info () const { return _info ; }

private:
    QWebSocketProtocol::CloseCode _err ;
    QString _info ;
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

    void onSettingsChanged () ;

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