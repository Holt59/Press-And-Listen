#include "PressAndListenQt.h"

#include <QtWidgets/QApplication>

#include "PressAndListenSettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PressAndListenQt w;
    w.show () ;
    return a.exec();
}
