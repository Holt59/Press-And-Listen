#ifndef _PRESSANDLISTEN_PLAYER_H
#define _PRESSANDLISTEN_PLAYER_H

#include <QtWebSockets/QWebSocket>
#include <QException>

struct PressAndListenSong {

    QString _id ;
    QString _title ;
    QString _artist ;
    bool _playing ;

    PressAndListenSong (QString const& id = QString (), 
                        QString const& title = QString (), 
                        QString const& artist = QString (), 
                        bool playing = false) : _id (id), _title (title), _artist (artist), _playing (playing) {}

    QString id () const { return _id ; }
    QString title () const { return _title ; }
    QString artist () const { return _artist ; }
    bool playing () const { return _playing ; }
    bool isValid () const { return _id != QString () ; }

    bool operator== (PressAndListenSong const& other) { return id () == other.id () ; }
    bool operator!= (PressAndListenSong const& other) { return !this->operator==(other) ; }

};

namespace PlayerInfo {

    enum class Browser {
        GOOGLE_CHROME,
        MOZILLA_FIREFOX,
        INTERNET_EXPLORER,
        OPERA,
        SAFARI
    };

    class UnrecognizedBrowser : public QException {
    public:
        UnrecognizedBrowser () : QException () {}
        virtual ~UnrecognizedBrowser () throw () {}
    };

    enum class Player {
        DEEZER,
        YOUTUBE,
        GROOVESHARK,
        SPOTIFY,
        GOOGLE_PLAY
    };

    class UnrecognizedPlayer : public QException {
    public:
        UnrecognizedPlayer () : QException () { }
        virtual ~UnrecognizedPlayer () throw () { }
    };

    QString toString (Browser browser) ;
    QString toString (Player player) ;

}

class PressAndListenPlayer : public QObject {

    friend class PressAndListenServer ;

    Q_OBJECT

public:

    PressAndListenPlayer (QWebSocket *socket, QObject *parent = 0);
    virtual ~PressAndListenPlayer ();

    bool isEnable () const { return m_enabled ; }
    bool isCurrentPlayer () const { return m_currentPlayer ; }
    PlayerInfo::Player player () const { return m_player ; }
    PlayerInfo::Browser browser () const { return m_browser ; }
    PressAndListenSong currentSong () const { return m_currentSong ; }

Q_SIGNALS:

    void songPaused (PressAndListenSong const& song) ;
    void songResume (PressAndListenSong const& song) ;
    void songChanged (PressAndListenSong const& oldSong, PressAndListenSong const& newSong) ;

    void initialized () ;
    void close () ;

    void update () ;

public Q_SLOTS:

    void toggle () ;
    void play () ;
    void pause () ;
    void prev () ;
    void next () ;

    void focusTab () ;
    void closeTab () ;

protected:

    void setEnabled () { m_enabled = true ; emit update () ; }
    void setDisabled () { m_enabled = false ; emit update () ; }
    void setIsCurrentPlayer (bool is = true) { m_currentPlayer = is ; emit update () ; }

private Q_SLOTS:

    void onMessageReceived (QString const& message) ;
    void socketDisconnected () ;

private:

    PlayerInfo::Player toPlayer (QString const& string) ;
    PlayerInfo::Browser toBrowser (QString const& string) ;

    QWebSocket *m_socket ;
    PlayerInfo::Player m_player ;
    PlayerInfo::Browser m_browser ;
    PressAndListenSong m_currentSong ;
    bool m_enabled, m_currentPlayer ;

};

#endif