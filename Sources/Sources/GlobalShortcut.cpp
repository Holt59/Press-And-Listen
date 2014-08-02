#include "GlobalShortcut.h"

#include <QApplication>
#include <QKeyEvent>

#include <QDebug>

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

#include"3rdparty/qxt/keymapper_x11.h"
#endif

QHash<QPair<quint32, quint32>, GlobalShortcut*> GlobalShortcut::shortcuts;

bool GlobalShortcut::m_isFilterInstalled = false;

int toNativeKey (int qtKey) {

#ifdef Q_OS_LINUX

        for (unsigned int i=1;i < sizeof(qtKey); i=i+2){
            if (KeyTbl[i] == qtKey){
                return KeyTbl[i-1];
            }
        }
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
            case Qt::Key_MediaLast:
                return VK_MEDIA_NEXT_TRACK;
            case Qt::Key_MediaPrevious:
                return VK_MEDIA_PREV_TRACK;
            case Qt::Key_MediaPlay:
            case Qt::Key_MediaRecord:
            case Qt::Key_MediaTogglePlayPause:
                return VK_MEDIA_PLAY_PAUSE;
            case Qt::Key_MediaStop:
                return VK_MEDIA_STOP;
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
                return qtKey;

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
                return qtKey;
        }
#endif

    return 0x0 ;
}

#ifdef Q_OS_LINUX

#define MOD_SHIFT ShiftMask
#define MOD_CONTROL ControlMask
#define MOD_ALT Mod1Mask
#define MOD_WIN Mod4Mask

#endif

int toNativeMods(int qtMods){
    int nativeMods = 0;

    if (qtMods & Qt::ShiftModifier)
        nativeMods |= MOD_SHIFT;
    if (qtMods & Qt::ControlModifier)
        nativeMods |= MOD_CONTROL;
    if (qtMods & Qt::AltModifier)
        nativeMods |= MOD_ALT;
    if (qtMods & Qt::MetaModifier)
        nativeMods |= MOD_WIN;

    return nativeMods;
}


GlobalShortcut::GlobalShortcut(QKeySequence shortcut, QObject *parent) :
    QObject (parent), m_shortcut (shortcut)
{

    if (!m_isFilterInstalled){
        QApplication * app = static_cast <QApplication*> (QApplication::instance());
        app->installNativeEventFilter(new GlobalShortcutEventFilter());
        m_isFilterInstalled = true;

    }

    int allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    int qtKey = shortcut[0] & ~(allMods); // Remove allMods on the key
    int qtMods = shortcut[0] &  allMods;

    m_key = toNativeKey(qtKey);

    if (m_key == 0x0) {
        throw KeyNotBoundException (shortcut, "Unable to find a corresponding native key.");
    }

    m_mods = toNativeMods(qtMods);

    this->registerKey(m_key, m_mods);

    shortcuts[QPair<quint32, quint32>(m_key, m_mods)] = this;
}



GlobalShortcut::~GlobalShortcut(){
    unregisterKey(m_key, m_mods);
}

#ifdef Q_OS_WIN

#define TO_HKEY_ID(k,m) ((nativeKey << 4) | (nativeMods & 0xf))

QString getLastWinError () {
    DWORD errorMessageID = ::GetLastError ();
    if (errorMessageID == 0)
        return QString () ;
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                  NULL, errorMessageID, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &messageBuffer, 0, NULL);
    QString msg = QString::fromLatin1 (messageBuffer, size);
    LocalFree (messageBuffer) ;
    return msg ;
}

#endif

void GlobalShortcut::registerKey(int nativeKey, int nativeMods){
#ifdef Q_OS_LINUX
    XGrabKey(QX11Info::display(), nativeKey, nativeMods, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(QX11Info::display(), nativeKey, nativeMods|Mod2Mask, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
    XSync(QX11Info::display(), False);
#endif

#ifdef Q_OS_WIN
    // TODO: Add information about the key
    if (!RegisterHotKey (NULL, TO_HKEY_ID(nativeKey, nativeMods), MOD_NOREPEAT | nativeMods, nativeKey)) { 
        throw KeyNotBoundException (m_shortcut, getLastWinError()) ; 
    }
#endif
}

void GlobalShortcut::unregisterKey(int nativeKey, int nativeMods){
#ifdef Q_OS_LINUX
    XUngrabKey(QX11Info::display(), nativeKey, nativeMods, QX11Info::appRootWindow());
    XUngrabKey(QX11Info::display(), nativeKey, nativeMods|Mod2Mask, QX11Info::appRootWindow()); // Accept if VerrNum is enabled
    XSync(QX11Info::display(), False);
#endif

#ifdef Q_OS_WIN
    UnregisterHotKey (0, TO_HKEY_ID (nativeKey, nativeMods));
#endif
}

void GlobalShortcut::trigger () {
    emit triggered ();
}
