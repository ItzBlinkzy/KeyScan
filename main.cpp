#include "KeyScan.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include "qtimer.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(".\\KeyScan.png"));

    splash->show();


    KeyScan w;
    QTimer::singleShot(2000, splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(show()));
    return a.exec();
}