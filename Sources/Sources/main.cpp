#include "pressandlistenqt.h"
#include "MediaKeyListener.h"

#include <QtWidgets/QApplication>

#include "PressAndListenSettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PressAndListenQt w;
    a.installNativeEventFilter (new MediaKeyListener (&w));
    w.show () ;
    return a.exec();
}
