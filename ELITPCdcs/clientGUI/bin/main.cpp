#include "DCSArt.h"
#include "ProjectVersion.h"
#include "configloader.h"
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>
#include <signal.h>
using json = nlohmann::json;
void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    QApplication::exit();
}

int main(int argc, char *argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    std::cout << DCSArt::ascii << DCS_VERSION << std::endl;

    json config = ConfigLoader::getMasterConfig(argc, argv);

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setWindowIcon(QIcon(":/images/res/dcs_64x64_bw.png"));
    QCoreApplication::setOrganizationName("FUW");
    QCoreApplication::setApplicationName("ELITPCdcs client");
    MainWindow w(config);
    w.show();
    return a.exec();
}
