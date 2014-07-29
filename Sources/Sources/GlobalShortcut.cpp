#include "GlobalShortcut.h"

#include <QApplication>
#include <QKeyEvent>

#include "GlobalShortcutEventFilter.h"

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

QHash<QPair<quint32, quint32>, GlobalShortcut*> GlobalShortcut::shortcuts;

bool GlobalShortcut::m_isFilterInstalled = False;

int toNativeKey(int qtKey){
#ifdef Q_OS_LINUX
        return XKeysymToKeycode(QX11Info::display(), XStringToKeysym(QKeySequence(qtKey).toString().toLatin1().data()));
#endif

#ifdef Q_OS_WIN

        //VK_*, described here :http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
        //Qt::Key_* described here : http://qt-project.org/doc/qt-5/qt.html#Key-enum
        switch (qtKey){
            case Qt::Key_Escape:
                return VK_ESCAPE;
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                return VK_TAB;
            case Qt::Key_Backspace:
                return VK_BACK;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                return VK_RETURN;
            case Qt::Key_Insert:
                return VK_INSERT;
            case Qt::Key_Delete:
                return VK_DELETE;
            case Qt::Key_Pause:
                return VK_PAUSE;
            case Qt::Key_Print:
                return VK_PRINT;
            case Qt::Key_Clear:
                return VK_CLEAR;
            case Qt::Key_Home:
                return VK_HOME;
            case Qt::Key_End:
                return VK_END;
            case Qt::Key_Left:
                return VK_LEFT;
            case Qt::Key_Up:
                return VK_UP;
            case Qt::Key_Right:
                return VK_RIGHT;
            case Qt::Key_Down:
                return VK_DOWN;
            case Qt::Key_PageUp:
                return VK_PRIOR;
            case Qt::Key_PageDown:
                return VK_NEXT;
            case Qt::Key_F1:
                return VK_F1;
            case Qt::Key_F2:
                return VK_F2;
            case Qt::Key_F3:
                return VK_F3;
            case Qt::Key_F4:
                return VK_F4;
            case Qt::Key_F5:
                return VK_F5;
            case Qt::Key_F6:
                return VK_F6;
            case Qt::Key_F7:
                return VK_F7;
            case Qt::Key_F8:
                return VK_F8;
            case Qt::Key_F9:
                return VK_F9;
            case Qt::Key_F10:
                return VK_F10;
            case Qt::Key_F11:
                return VK_F11;
            case Qt::Key_F12:
                return VK_F12;
            case Qt::Key_F13:
                return VK_F13;
            case Qt::Key_F14:
                return VK_F14;
            case Qt::Key_F15:
                return VK_F15;
            case Qt::Key_F16:
                return VK_F16;
            case Qt::Key_F17:
                return VK_F17;
            case Qt::Key_F18:
                return VK_F18;
            case Qt::Key_F19:
                return VK_F19;
            case Qt::Key_F20:
                return VK_F20;
            case Qt::Key_F21:
                return VK_F21;
            case Qt::Key_F22:
                return VK_F22;
            case Qt::Key_F23:
                return VK_F23;
            case Qt::Key_F24:
                return VK_F24;
            case Qt::Key_Space:
                return VK_SPACE;
            case Qt::Key_Asterisk:
                return VK_MULTIPLY;
            case Qt::Key_Plus:
                return VK_ADD;
            case Qt::Key_Comma:
                return VK_SEPARATOR;
            case Qt::Key_Minus:
                return VK_SUBTRACT;
            case Qt::Key_Slash:
                return VK_DIVIDE;
            case Qt::Key_MediaNext:
                return VK_MEDIA_NEXT_TRACK;
            case Qt::Key_MediaPrevious:
                return VK_MEDIA_PREV_TRACK;
            case Qt::Key_MediaPlay:
                return VK_MEDIA_PLAY_PAUSE;
            case Qt::Key_MediaStop:
                return VK_MEDIA_STOP;
                // couldn't find those in VK_*
                //case Qt::Key_MediaLast:
                //case Qt::Key_MediaRecord:
            case Qt::Key_VolumeDown:
                return VK_VOLUME_DOWN;
            case Qt::Key_VolumeUp:
                return VK_VOLUME_UP;
            case Qt::Key_VolumeMute:
                return VK_VOLUME_MUTE;

                // numbers
            case Qt::Key_0:
            case Qt::Key_1:
            case Qt::Key_2:
            case Qt::Key_3:
            case Qt::Key_4:
            case Qt::Key_5:
            case Qt::Key_6:
            case Qt::Key_7:
            case Qt::Key_8:
            case Qt::Key_9:
                return key;

                // letters
            case Qt::Key_A:
            case Qt::Key_B:
            case Qt::Key_C:
            case Qt::Key_D:
            case Qt::Key_E:
            case Qt::Key_F:
            case Qt::Key_G:
            case Qt::Key_H:
            case Qt::Key_I:
            case Qt::Key_J:
            case Qt::Key_K:
            case Qt::Key_L:
            case Qt::Key_M:
            case Qt::Key_N:
            case Qt::Key_O:
            case Qt::Key_P:
            case Qt::Key_Q:
            case Qt::Key_R:
            case Qt::Key_S:
            case Qt::Key_T:
            case Qt::Key_U:
            case Qt::Key_V:
            case Qt::Key_W:
            case Qt::Key_X:
            case Qt::Key_Y:
            case Qt::Key_Z:
                return key;

            default:
                throw KeyNotBoundException ();
                return 0;
        }
#endif
}


int toNativeMods(int qtMods){
    int nativeMods = 0;

#ifdef Q_OS_LINUX
    if (qtMods & Qt::ShiftModifier)
        nativeMods |= ShiftMask;
    if (qtMods & Qt::ControlModifier)
        nativeMods |= ControlMask;
    if (qtMods & Qt::AltModifier)
        nativeMods |= Mod1Mask;
    if (qtMods & Qt::MetaModifier)
        nativeMods |= Mod4Mask;
#endif

#ifdef Q_OS_WIN
    if (qtMods & Qt::ShiftModifier)
        nativeMods |= MOD_SHIFT;
    if (qtMods & Qt::ControlModifier)
        nativeMods |= MOD_CONTROL;
    if (qtMods & Qt::AltModifier)
        nativeMods |= MOD_ALT;
    if (qtMods & Qt::MetaModifier)
        nativeMods |= MOD_WIN;
#endif
    return nativeMods;
}


GlobalShortcut::GlobalShortcut(QKeySequence shortcut) :
    QObject(0)
{

    if (!m_isFilterInstalled){
        QApplication * app = static_cast <QApplication*> (QApplication::instance());
        app->installNativeEventFilter(new GlobalShortcutEventFilter());
        m_isFilterInstalled = True;

    }
    int allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    int qtKey = shortcut[0] & ~(allMods); // Remove allMods on the key
    int qtMods = shortcut[0] &  allMods;

    m_key = toNativeKey(qtKey);
    m_mods = toNativeMods(qtMods);

    this->registerKey(m_key, m_mods);

    shortcuts[QPair<quint32, quint32>(m_key,m_mods)] = this;
}



GlobalShortcut::~GlobalShortcut(){
    unregisterKey(m_key, m_mods);
}

void GlobalShortcut::setHotKey(QKeySequence shortcut){
}

void GlobalShortcut::registerKey(int nativeKey, int nativeMods){
#ifdef Q_OS_LINUX
    XGrabKey(QX11Info::display(), nativeKey, nativeMods, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(QX11Info::display(), nativeKey, nativeMods|Mod2Mask, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
    XSync(QX11Info::display(), False);
#endif

#ifdef Q_OS_WIN
    if (!RegisterHotKey (NULL, nativeKey | nativeMods, MOD_NOREPEAT | nativeMods, nativeKey)) { throw KeyNotBoundException () ; }
#endif
}


void GlobalShortcut::unregisterKey(int nativeKey, int nativeMods){
#ifdef Q_OS_LINUX
    XUngrabKey(QX11Info::display(), nativeKey, nativeMods, QX11Info::appRootWindow());
    XUngrabKey(QX11Info::display(), nativeKey, nativeMods|Mod2Mask, QX11Info::appRootWindow()); // Accept if VerrNum is enabled
    XSync(QX11Info::display(), False);
#endif

#ifdef Q_OS_WIN
    UnregisterHotKey(0, nativeKey);
#endif
}

void GlobalShortcut::activate(){
    emit activated();
}