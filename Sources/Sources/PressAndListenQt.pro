TEMPLATE = app
TARGET = PressAndListenQt
win32:DESTDIR = ../Win32/Release
QT += core websockets widgets gui
unix:QT += x11extras
QT += gui-private

CONFIG += release
CONFIG += embed_manifest_exe
CONFIG += c++11
DEFINES += QT_DLL QT_NETWORK_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = PressAndListenQt.rc

LIBS += -lX11
HEADERS += \
    PlayerListWidget.h \
    PressAndListenQt.h \
    PressAndListenPlayer.h \
    PressAndListenSettings.h \
    NotificationSettingsDialog.h \
    PlayerItemWidget.h \
    PressAndListenServer.h \
    GlobalShortcut.h \
    GlobalShortcutEventFilter.h

SOURCES += \
    PressAndListenPlayer.cpp \
    NotificationSettingsDialog.cpp \
    PlayerListWidget.cpp \
    PressAndListenQt.cpp \
    PressAndListenServer.cpp \
    PlayerItemWidget.cpp \
    PressAndListenSettings.cpp \
    main.cpp \
    GlobalShortcut.cpp \
    GlobalShortcutEventFilter.cpp
