#include "GlobalShortcutEventFilter.h"

#include <QDebug>
#include <QCoreApplication>
#include "GlobalShortcut.h"

#ifdef Q_OS_WIN

#include <windows.h>

#endif

#ifdef Q_OS_LINUX

#include <QX11Info>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <xcb/xcb.h>

#endif

GlobalShortcutEventFilter::GlobalShortcutEventFilter() :
    QAbstractNativeEventFilter()
{
}

bool GlobalShortcutEventFilter::nativeEventFilter (const QByteArray & eventType, void * message, long * result) {

    int keycode = 0;
    int modifiers = 0;

#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast <MSG *> (message) ;

        if (msg->message == WM_HOTKEY) {
            keycode = msg->wParam >> 4;
            modifiers = msg->wParam & 0xf;
            qDebug () << hex << keycode << ", " << modifiers ;
        }
    }

#endif

#ifdef Q_OS_LINUX
  if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t *msg = static_cast <xcb_generic_event_t*> (message) ;
        if((msg->response_type&127) == XCB_KEY_PRESS){
            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)msg;
            int allMods = ShiftMask | ControlMask | Mod1Mask | Mod4Mask;

            keycode = kp->detail;
            modifiers = kp->state & allMods; // Ignore other modifiers like Verr_Num
        }
    }
#endif

  if (keycode > 0) {
      GlobalShortcut* shortcut = GlobalShortcut::shortcuts.value(QPair<quint32, quint32> (keycode, modifiers));
      if (shortcut) {
          shortcut->activate();
       }
  }

  return false;
}

