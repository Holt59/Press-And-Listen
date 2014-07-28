#include "PressAndListenSettings.h"

QList <PressAndListenSettings::NotificationType> PressAndListenSettings::getNotificationTypes () {
    return {
        NotificationType::PlayerEnter,
        NotificationType::PlayerLeave,
        NotificationType::PlayerSleep,
        NotificationType::PlayerWakeUp,
        NotificationType::PlayerResume,
        NotificationType::PlayerPaused,
        NotificationType::PlayerSwitchSong,
        NotificationType::PlayerSwitchPlayer
    } ;
}

QString PressAndListenSettings::toUserString (PressAndListenSettings::NotificationType const& notification) {
    switch (notification) {
        case NotificationType::PlayerEnter: return tr("A new player is detected") ;
        case NotificationType::PlayerLeave: return tr ("A player disconnect") ;
        case NotificationType::PlayerSleep: return tr ("A player is pause (control mode)") ;
        case NotificationType::PlayerWakeUp: return tr ("A player is resume (control mode)") ;
        case NotificationType::PlayerResume: return tr ("A song is playing") ;
        case NotificationType::PlayerPaused: return tr ("A song is paused") ;
        case NotificationType::PlayerSwitchSong: return tr ("A new song is playing") ;
        case NotificationType::PlayerSwitchPlayer: return tr ("The current player is changed") ;
    }
    return QString () ;
}

PressAndListenSettings::PressAndListenSettings (QObject *parent) :
QSettings(QSettings::IniFormat, QSettings::UserScope, "Press & Listen", "Press & Listen", parent) {

}

PressAndListenSettings::~PressAndListenSettings () {
    this->sync () ;
}

bool PressAndListenSettings::showSleepingPlayer () {
    return this->value ("General/ShowSleepingPlayer", QVariant(true)).toBool () ;
}

QString PressAndListenSettings::toString (NotificationType const& notification) {
    switch (notification) {
        case NotificationType::PlayerEnter: return "PlayerEnter" ;
        case NotificationType::PlayerLeave: return "PlayerLeave" ;
        case NotificationType::PlayerSleep: return "PlayerSleep" ;
        case NotificationType::PlayerWakeUp: return "PlayerWakeUp" ;
        case NotificationType::PlayerResume: return "PlayerResume" ;
        case NotificationType::PlayerPaused: return "PlayerPaused" ;
        case NotificationType::PlayerSwitchSong: return "PlayerSwitchSong" ;
        case NotificationType::PlayerSwitchPlayer: return "PlayerSwitchPlayer" ;
    }
    return QString () ;
}


bool PressAndListenSettings::showNotification (NotificationType const& notification) {
    return this->value (QString ("Notifications/") + toString (notification), QVariant (true)).toBool () ;
}

void PressAndListenSettings::setShowNotification (NotificationType const& notification, bool newValue) {
    this->setValue (QString ("Notifications/") + toString (notification), QVariant (newValue)) ;
}

bool PressAndListenSettings::isEnable (PlayerInfo::Player const& player) {
    return this->value (QString("Players/") + PlayerInfo::toString(player), QVariant (true)).toBool () ;
}