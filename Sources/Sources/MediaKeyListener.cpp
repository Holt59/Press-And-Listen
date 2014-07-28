#include "MediaKeyListener.h"

#ifdef Q_OS_WIN

#include <windows.h>

#define TO_WIN_KNUM(v) (v - QEvent::Type::User)
#define TO_WIN_HNUM(v) (QEvent::Type::User + v)

#endif

#include <QDebug>

MediaKeyListener::MediaKeyListener (QObject *receiver) : QAbstractNativeEventFilter (), m_receiver(receiver) {

#if defined Q_OS_WIN

    if (!RegisterHotKey (NULL, TO_WIN_KNUM(HotKey::Stop), MOD_NOREPEAT, VK_MEDIA_STOP)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM(HotKey::Play), MOD_NOREPEAT, VK_MEDIA_PLAY_PAUSE)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::Prev), MOD_NOREPEAT, VK_MEDIA_PREV_TRACK)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::Next), MOD_NOREPEAT, VK_MEDIA_NEXT_TRACK)) { throw KeyNotBoundException () ; }

    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::AltStop), MOD_ALT | MOD_NOREPEAT, VK_MEDIA_STOP)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::AltPlay), MOD_ALT | MOD_NOREPEAT, VK_MEDIA_PLAY_PAUSE)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::AltPrev), MOD_ALT | MOD_NOREPEAT, VK_MEDIA_PREV_TRACK)) { throw KeyNotBoundException () ; }
    if (!RegisterHotKey (NULL, TO_WIN_KNUM (HotKey::AltNext), MOD_ALT | MOD_NOREPEAT, VK_MEDIA_NEXT_TRACK)) { throw KeyNotBoundException () ; }

    QEvent::registerEventType (HotKey::Stop);
    QEvent::registerEventType (HotKey::Play);
    QEvent::registerEventType (HotKey::Prev);
    QEvent::registerEventType (HotKey::Next);
    QEvent::registerEventType (HotKey::AltStop);
    QEvent::registerEventType (HotKey::AltPlay);
    QEvent::registerEventType (HotKey::AltPrev);
    QEvent::registerEventType (HotKey::AltNext);

#endif
    
}

MediaKeyListener::~MediaKeyListener () {}

bool MediaKeyListener::nativeEventFilter (const QByteArray & eventType, void * message, long * result) {

    int  key = QEvent::User ;

    if (eventType == "windows_generic_MSG") {

#ifdef Q_OS_WIN

        MSG *msg = static_cast <MSG *> (message) ;

        if (msg->message == WM_HOTKEY) {
            key = TO_WIN_HNUM(msg->wParam) ;
        }

#endif

    }

    if (eventType == "xcb_generic_event_t") {

#ifdef Q_WS_X11

        xcb_generic_event_t *msg = static_cast <xcb_generic_event_t*> (message) ;

#endif

    }

    if (key != QEvent::User) {
        QCoreApplication::sendEvent (m_receiver, new QEvent (static_cast <QEvent::Type> (key)));
    }

    return false ;
}