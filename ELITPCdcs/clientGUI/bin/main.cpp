#include "mainwindow.h"
#include "configloader.h"
#include <QApplication>
#include <QStyleFactory>
#include <signal.h>
#include "DCSArt.h"
using json = nlohmann::json;
void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    QApplication::exit();
    }

int main(int argc, char *argv[]){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    DCSArt::header();

  json config=ConfigLoader::getMasterConfig(argc,argv);

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setWindowIcon(QIcon(":/images/res/dcs_64x64_bw.png"));
    QCoreApplication::setOrganizationName("FUW");
    QCoreApplication::setApplicationName("ELITPCdcs client");
    MainWindow w(config);
    w.show();
    return a.exec();
}
