#include "PressAndListenServer.h"

#include <QEventLoop>
#include <QFile>
#include <QSslKey>

#include "PressAndListenSettings.h"

PressAndListenServer::PressAndListenServer (quint16 port, QObject *parent) : 
    QWebSocketServer("Press And Listen Server", QWebSocketServer::NonSecureMode, parent), m_currentPlayer(0) {

    if (!this->listen (QHostAddress::Any, port)) {
        throw ServerNotStartedException () ;
    }
    qDebug () << "Server listenning on: " << port ;
    connect (this, &QWebSocketServer::newConnection, this, &PressAndListenServer::onNewConnection);
    connect (this, &QWebSocketServer::closed, this, &PressAndListenServer::onClose);
}

PressAndListenServer::~PressAndListenServer () {}

PressAndListenPlayer* PressAndListenServer::prevPlayer () {
    PressAndListenPlayer *oldPlayer = this->currentPlayer () ;
    if (!m_players.isEmpty ()) {
        m_currentPlayer = (m_currentPlayer == 0) ? m_players.size () - 1 : m_currentPlayer - 1;
        while (oldPlayer != this->currentPlayer ()
               && !this->currentPlayer ()->isEnable ()) {
            m_currentPlayer = (m_currentPlayer == 0) ? m_players.size () - 1 : m_currentPlayer - 1;
        }
    }
    if (oldPlayer != this->currentPlayer ()) {
        oldPlayer->setIsCurrentPlayer (false);
        this->currentPlayer ()->setIsCurrentPlayer (true);
    }
    return this->currentPlayer () ;
}
PressAndListenPlayer* PressAndListenServer::nextPlayer () {
    PressAndListenPlayer *oldPlayer = this->currentPlayer () ;
    if (!m_players.isEmpty ()) {
        m_currentPlayer = (m_currentPlayer + 1) % m_players.size () ;
        while (oldPlayer != this->currentPlayer ()
               && !this->currentPlayer ()->isEnable ()) {
            m_currentPlayer = (m_currentPlayer + 1) % m_players.size () ;
        }
    }
    if (oldPlayer != this->currentPlayer ()) {
        oldPlayer->setIsCurrentPlayer (false);
        this->currentPlayer ()->setIsCurrentPlayer (true);
    }
    return this->currentPlayer () ;
}

PressAndListenPlayer *PressAndListenServer::setCurrentPlayer (PressAndListenPlayer *player) {
    PressAndListenPlayer *oldPlayer = this->currentPlayer () ;
    int index = m_players.indexOf (player);
    if (index < 0) {
        return nullptr ;
    }
    m_currentPlayer = index ;
    oldPlayer->setIsCurrentPlayer (false);
    this->currentPlayer ()->setIsCurrentPlayer (true);
    return this->currentPlayer () ;
}


void PressAndListenServer::onUserSleepPlayer (PressAndListenPlayer *player) {
    player->setDisabled () ;
    if (this->currentPlayer () == player) {
        this->nextPlayer () ;
    }
}

void PressAndListenServer::onUserWakeUpPlayer (PressAndListenPlayer *player) {
    player->setEnabled () ;
}


void PressAndListenServer::onNewConnection () {
    PressAndListenPlayer *player = new PressAndListenPlayer (this->nextPendingConnection ()) ;
    qDebug () << "Client connected: " << player->m_socket->peerName () << player->m_socket->origin() ;
    {
        QEventLoop loop;
        loop.connect (player, &PressAndListenPlayer::initialized, &loop, &QEventLoop::quit);
        loop.exec ();
    }
    if (SETTINGS.isEnable (player->player ())) {
        m_players.push_back (player);
        connect (player, &PressAndListenPlayer::songChanged, this, &PressAndListenServer::onSongChanged) ;
        connect (player, &PressAndListenPlayer::close, this, &PressAndListenServer::onClose) ;
        emit playerEnter (player);
        if (m_players.size () == 1) {
            player->setIsCurrentPlayer (true);
        }
        else if (currentPlayer()->currentSong().playing()) {
            player->pause () ;
        }
    }
    else {
        player->deleteLater () ;
    }
}

void PressAndListenServer::onSongChanged (PressAndListenSong const& oldSong, PressAndListenSong const& newSong) {
    PressAndListenPlayer *player = qobject_cast <PressAndListenPlayer*> (sender ()) ;
    if (!this->currentPlayer()->currentSong().isValid () && newSong.isValid () && newSong.playing ()
        && player != m_players[m_currentPlayer]) {
        this->currentPlayer ()->pause () ;
        this->setCurrentPlayer (player);
    }
}


void PressAndListenServer::onClose () {
    PressAndListenPlayer *pClient = qobject_cast <PressAndListenPlayer *> (sender ());
    emit playerLeave (pClient) ;
    m_players.removeAll (pClient) ;
    pClient->deleteLater () ;
    if (m_currentPlayer >= m_players.size ()) {
        this->setCurrentPlayer (m_players[0]) ;
    }
}