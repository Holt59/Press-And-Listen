#include "PressAndListenSettings.h"

PressAndListenSettings SETTINGS ;

#if NO_MEDIA_KEYS
#define SHORTCUT_TOGGLE Qt::ControlModifier | Qt::MetaModifier | Qt::Key_Up
#define SHORTCUT_NEXT Qt::ControlModifier | Qt::MetaModifier | Qt::Key_Right
#define SHORTCUT_PREVIOUS Qt::ControlModifier | Qt::MetaModifier | Qt::Key_Left
#define SHORTCUT_NEXT_PLAYER Qt::AltModifier | Qt::ControlModifier | Qt::MetaModifier | Qt::Key_Right
#define SHORTCUT_PREVIOUS_PLAYER Qt::AltModifier | Qt::ControlModifier | Qt::MetaModifier | Qt::Key_Left
#else
#define SHORTCUT_TOGGLE Qt::Key_MediaTogglePlayPause
#define SHORTCUT_NEXT Qt::Key_MediaNext
#define SHORTCUT_PREVIOUS Qt::Key_MediaPrevious
#define SHORTCUT_NEXT_PLAYER Qt::AltModifier | Qt::Key_MediaNext
#define SHORTCUT_PREVIOUS_PLAYER Qt::AltModifier | Qt::Key_MediaPrevious
#endif

QMap <PressAndListenSettings::ShortcutType, QKeySequence> PressAndListenSettings::DEFAULT_SHORTCUTS {
        {PressAndListenSettings::ShortcutType::Toggle, QKeySequence (SHORTCUT_TOGGLE)},
        {PressAndListenSettings::ShortcutType::Next, QKeySequence (SHORTCUT_NEXT)},
        {PressAndListenSettings::ShortcutType::Previous, QKeySequence (SHORTCUT_PREVIOUS)},
        {PressAndListenSettings::ShortcutType::NextPlayer, QKeySequence (SHORTCUT_NEXT_PLAYER)},
        {PressAndListenSettings::ShortcutType::PreviousPlayer, QKeySequence (SHORTCUT_PREVIOUS_PLAYER)}
} ;

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

QList <PressAndListenSettings::ShortcutType> PressAndListenSettings::getShortcutTypes () {
    return {
        ShortcutType::Toggle,
        ShortcutType::Next,
        ShortcutType::Previous,
        ShortcutType::PreviousPlayer,
        ShortcutType::NextPlayer
    } ;
}

QString PressAndListenSettings::toUserString (ShortcutType const& shortcut) {
    switch (shortcut) {
        case ShortcutType::Toggle: return tr ("Toggle play/pause on current player") ;
        case ShortcutType::Next: return tr ("Go to next song") ;
        case ShortcutType::Previous: return tr ("Go to previous song") ;
        case ShortcutType::PreviousPlayer: return tr ("Switch to previous player") ;
        case ShortcutType::NextPlayer: return tr ("Switch to next player") ;
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

QString PressAndListenSettings::toString (ShortcutType const& shortcut) {
    switch (shortcut) {
        case ShortcutType::Toggle: return "Toggle" ;
        case ShortcutType::Next: return "Next" ;
        case ShortcutType::Previous: return "Previous" ;
        case ShortcutType::PreviousPlayer: return "PreviousPlayer" ;
        case ShortcutType::NextPlayer: return "NextPlayer" ;
    }
    return QString () ;
}

QKeySequence PressAndListenSettings::getShortcut (ShortcutType const& shortcut) {
    return QKeySequence (this->value (QString ("Shortcuts/" + this->toString (shortcut)), QVariant (DEFAULT_SHORTCUTS[shortcut].toString())).toString()) ;
}