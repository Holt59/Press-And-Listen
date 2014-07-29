TEMPLATE = app
TARGET = PressAndListenQt
QT += core websockets widgets gui
unix:QT += x11extras
QT += gui-private

CONFIG += release
CONFIG += embed_manifest_exe
CONFIG += c++11
DEFINES += QT_DLL QT_NETWORK_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ../Sources \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = PressAndListenQt.rc

LIBS += -lX11
HEADERS += \
    ../Sources/PlayerListWidget.h \
    ../Sources/PressAndListenQt.h \
    ../Sources/PressAndListenPlayer.h \
    ../Sources/PressAndListenSettings.h \
    ../Sources/NotificationSettingsDialog.h \
    ../Sources/PlayerItemWidget.h \
    ../Sources/PressAndListenServer.h \
    ../Sources/GlobalShortcut.h \
    ../Sources/GlobalShortcutEventFilter.h

SOURCES += \
    ../Sources/PressAndListenPlayer.cpp \
    ../Sources/NotificationSettingsDialog.cpp \
    ../Sources/PlayerListWidget.cpp \
    ../Sources/PressAndListenQt.cpp \
    ../Sources/PressAndListenServer.cpp \
    ../Sources/PlayerItemWidget.cpp \
    ../Sources/PressAndListenSettings.cpp \
    ../Sources/main.cpp \
    ../Sources/GlobalShortcut.cpp \
    ../Sources/GlobalShortcutEventFilter.cpp
