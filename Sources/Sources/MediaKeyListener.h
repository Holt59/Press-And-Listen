#ifndef _MEDIAKEYLISTENER_H
#define _MEDIAKEYLISTENER_H

#include <QAbstractNativeEventFilter>
#include <QCoreApplication>
#include <QException>
#include <QEvent>

class KeyNotBoundException : public QException {
public:
    KeyNotBoundException () : QException () {}
    virtual ~KeyNotBoundException () throw () {}
};

class MediaKeyListener : public QAbstractNativeEventFilter {

public:

    enum HotKey {
        Stop = QEvent::User + 1,
        Play = QEvent::User + 2,
        Prev = QEvent::User + 3,
        Next = QEvent::User + 4,
        AltStop = QEvent::User + 5,
        AltPlay = QEvent::User + 6,
        AltPrev = QEvent::User + 7,
        AltNext = QEvent::User + 8
    };

public:

    MediaKeyListener (QObject *receiver) ;
    virtual ~MediaKeyListener () ;

protected:

    virtual bool nativeEventFilter (const QByteArray & eventType, void * message, long * result) Q_DECL_OVERRIDE ;

private:

    QObject *m_receiver ;

};

#endif

