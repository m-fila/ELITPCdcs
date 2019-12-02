#include "include/mainwindow.h"
#include "configWrapper.h"
#include <QApplication>
#include <QStyleFactory>
#include <signal.h>
void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    QApplication::exit();
    }
int main(int argc, char *argv[]){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    
   /*Loader loader(argc, argv);
    if(loader.isOpened()){
        loader.parse();
    }
    else{
        std::cout<<"Can't open config file"<<std::endl;
        return 0;
    }
*/
  ConfigWrapper configs;
  Json::Value root;
  if(argc==2){
    std::ifstream ifs(static_cast<std::string>(argv[1]));
    root=configs.parse(ifs);
  }
  else{
    std::cout<<"Can't open config file"<<std::endl;
    return 0;
  }

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    QCoreApplication::setOrganizationName("FUW");
    QCoreApplication::setApplicationName("ELITPCdcs client");
    MainWindow w(root);
    w.show();
    return a.exec();
}
