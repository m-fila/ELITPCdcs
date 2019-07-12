#include "include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("FUW");
    QCoreApplication::setApplicationName("ELITPCdcs client");
    MainWindow w;
    w.show();

    return a.exec();
}
