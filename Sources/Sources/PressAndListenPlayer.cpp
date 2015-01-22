#include "PressAndListenPlayer.h"

#include <QJsonDocument>
#include <QJsonObject>

PressAndListenPlayer::PressAndListenPlayer (QWebSocket *socket, QObject *parent) :
QObject (parent), m_socket (socket), m_enabled (true), m_currentPlayer (false), m_currentSong (PressAndListenSong ()) {

    connect (m_socket, &QWebSocket::textMessageReceived, this, &PressAndListenPlayer::onMessageReceived);
    connect (m_socket, &QWebSocket::disconnected, this, &PressAndListenPlayer::socketDisconnected);

}


PressAndListenPlayer::~PressAndListenPlayer () { 
    m_socket->close () ;
}

void PressAndListenPlayer::toggle () { m_socket->sendTextMessage ("toggle"); }
void PressAndListenPlayer::play () { m_socket->sendTextMessage ("play"); }
void PressAndListenPlayer::pause () { m_socket->sendTextMessage ("pause"); }
void PressAndListenPlayer::prev () { m_socket->sendTextMessage ("prev"); }
void PressAndListenPlayer::next () { m_socket->sendTextMessage ("next"); }

void PressAndListenPlayer::focusTab () { m_socket->sendTextMessage ("focusTab"); }
void PressAndListenPlayer::closeTab () { m_socket->sendTextMessage ("closeTab"); }

PlayerInfo::Player PressAndListenPlayer::toPlayer (QString const& str) {
    if (str == "deezer") {
        return PlayerInfo::Player::DEEZER ;
    }
    if (str == "youtube") {
        return PlayerInfo::Player::YOUTUBE ;
    }
    if (str == "grooveshark") {
        return PlayerInfo::Player::GROOVESHARK ;
    }
    if (str == "spotify") {
        return PlayerInfo::Player::SPOTIFY ;
    }/*
    if (str == "gplay") {
        return PlayerInfo::Player::GOOGLE_PLAY ;
    } */
    throw PlayerInfo::UnrecognizedPlayer () ;
}

QList <PlayerInfo::Player> PlayerInfo::getPlayers () {
    return {
        PlayerInfo::Player::DEEZER,
        PlayerInfo::Player::YOUTUBE,
        PlayerInfo::Player::GROOVESHARK,
        PlayerInfo::Player::SPOTIFY
    } ;
}

PlayerInfo::Browser PressAndListenPlayer::toBrowser (QString const& str) {
    if (str == "chrome") {
        return PlayerInfo::Browser::GOOGLE_CHROME ;
    }
    if (str == "firefox") {
        return PlayerInfo::Browser::MOZILLA_FIREFOX ;
    }
    if (str == "iexplorer") {
        return PlayerInfo::Browser::INTERNET_EXPLORER ;
    }
    if (str == "opera") {
        return PlayerInfo::Browser::OPERA ;
    }
    if (str == "safari") {
        return PlayerInfo::Browser::SAFARI ;
    }
    throw PlayerInfo::UnrecognizedBrowser () ;
}

void PressAndListenPlayer::onMessageReceived (QString const& message) {
    QJsonObject object = QJsonDocument::fromJson (message.toUtf8 ()).object () ;
    QString command = object.value (QString ("command")).toString () ;
    if (command == "initialized") {
        qDebug () << "In command: " << message ;
        m_player = toPlayer (object.value (QString ("player")).toString ()) ;
        m_browser = toBrowser (object.value (QString ("browser")).toString ()) ; 
        emit initialized () ;
    }
    else if (command == "songinfo") {
        PressAndListenSong oldSong = m_currentSong ;
        bool playing = object.value (QString ("playing")).toBool () ;
        QJsonObject songInfo = object.value (QString ("info")).toObject () ;
        if (songInfo.keys ().empty ()) {
            m_currentSong = PressAndListenSong () ;
        }
        else {
            m_currentSong = PressAndListenSong (songInfo.value (QString ("id")).toString (),
                                                songInfo.value (QString ("title")).toString (),
                                                songInfo.value (QString ("artist")).toString (),
                                                playing);
        }
        if (m_currentSong != oldSong) {
            emit songChanged (oldSong, m_currentSong) ;
        }
        else if (oldSong.playing () && !m_currentSong.playing ()) {
            emit songPaused (m_currentSong) ;
        }
        else if (!oldSong.playing () && m_currentSong.playing ()) {
            emit songResume (m_currentSong) ;
        }
    }
}

void PressAndListenPlayer::socketDisconnected () {
    emit close () ;
}

QString PlayerInfo::toString (Browser browser) {
    switch (browser) {
        case Browser::GOOGLE_CHROME:
            return QString ("Google Chrome") ;
        case Browser::MOZILLA_FIREFOX:
            return QString ("Mozilla Firefox") ;
        case Browser::INTERNET_EXPLORER:
            return QString ("Internet Explorer") ;
        case Browser::OPERA:
            return QString ("Opera") ;
        case Browser::SAFARI:
            return QString ("Safari") ;
    }
    return QString () ;
}

QString PlayerInfo::toString (Player player) {
    switch (player) {
        case Player::DEEZER:
            return QString("Deezer") ;
        case Player::YOUTUBE:
            return QString ("Youtube") ;
        case Player::GROOVESHARK:
            return QString ("Grooveshark") ;
        case Player::SPOTIFY:
            return QString ("Spotify") ; /*
        case Player::GOOGLE_PLAY:
            return QString ("Google Play") ; */
    }
    return QString () ;
}