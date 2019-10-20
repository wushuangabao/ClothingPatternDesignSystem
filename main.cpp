#include "mainwindow.h"
#include "splash/splash.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Splash s;
    s.show();
    MainWindow mainWin;
    s.waitFor(&mainWin,3000);

    return a.exec();
}
