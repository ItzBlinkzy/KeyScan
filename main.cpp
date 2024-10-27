#include "KeyScan.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyScan w;
    w.show();
    return a.exec();
}
