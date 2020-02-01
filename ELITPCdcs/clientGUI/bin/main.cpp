#include "mainwindow.h"
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
    
  ConfigWrapper configs;
  Json::Value root;
  std::string configPath;
  if(argc>1){
    configPath=argv[1];
  }
  else{
    std::string homePath=getenv("HOME");
    configPath=homePath+"/.dcs/dcs.json";
  }
  std::ifstream ifs(configPath);
  if(ifs.is_open()){
    root=configs.parse(ifs);
  }
  else{
    std::cout<<"Can't find config file"<<std::endl;
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
