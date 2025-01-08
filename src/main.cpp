#include "KeyScan.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include "qtimer.h"
#include "qdir.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/KeyScan.png").scaled(200, 200));
    splash->show();
    

    KeyScan w;
    QTimer::singleShot(2000, splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));
    return a.exec();
}