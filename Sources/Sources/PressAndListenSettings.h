#ifndef _PRESSANDLISTENSETTINGS_H
#define _PRESSANDLISTENSETTINGS_H

#include <QSettings>
#include <QKeySequence>

#include "PressAndListenPlayer.h"

class PressAndListenSettings : public QSettings {

public:

    enum class NotificationType {
        PlayerEnter,
        PlayerLeave,
        PlayerSleep,
        PlayerWakeUp,
        PlayerResume,
        PlayerPaused,
        PlayerSwitchSong,
        PlayerSwitchPlayer
    } ;

    enum class ShortcutType {
        Toggle,
        Previous,
        Next,
        PreviousPlayer,
        NextPlayer
    } ;

public:

    static QList <NotificationType> getNotificationTypes ();
    static QString toUserString (NotificationType const&);

    static QList <ShortcutType> getShortcutTypes ();
    static QString toUserString (ShortcutType const &) ;

    PressAndListenSettings (QObject * parent = 0);
    ~PressAndListenSettings ();

    bool showSleepingPlayer () ;
    bool showNotification (NotificationType const&) ;
    void setShowNotification (NotificationType const&, bool) ;

    bool isEnable (PlayerInfo::Player const&) ;

    QList <QKeySequence> getShortcut (ShortcutType const&) ;

private:

    /* Return parameter name. */
    QString toString (NotificationType const&) ;
    QString toString (ShortcutType const&) ;

    static QMap <ShortcutType, QList <QKeySequence>> DEFAULT_SHORTCUTS ;

};

extern PressAndListenSettings SETTINGS ;

#endif