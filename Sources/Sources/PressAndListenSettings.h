#ifndef _PRESSANDLISTENSETTINGS_H
#define _PRESSANDLISTENSETTINGS_H

#include <QSettings>

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

public:

    static QList <NotificationType> getNotificationTypes ();
    static QString toUserString (NotificationType const&);

    PressAndListenSettings (QObject * parent = 0);
    ~PressAndListenSettings ();

    bool showSleepingPlayer () ;
    bool showNotification (NotificationType const&) ;
    void setShowNotification (NotificationType const&, bool) ;

    bool isEnable (PlayerInfo::Player const&) ;

private:

    /* Return parameter name. */
    QString toString (NotificationType const&) ;

};

typedef PressAndListenSettings PSettings ;

#endif