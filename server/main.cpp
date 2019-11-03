#include "include/opc/opc_server.h"
#include "include/opc/opc_state.h"
#include "include/opc/hmp2020controller.h"
#include "include/opc/dt1415controller.h"
#include "../common/loader.h"
#include <thread>
int main(int argc, char *argv[]){
  Loader loader(argc, argv);
  if(loader.isOpened()){
    loader.parse();
  }
  else{
    std::cout<<"Can't open config file"<<std::endl;
    return 0;
  }

  OpcServer server(loader.getAddress(),loader.getPort());
  OpcState state;
  state.init(server.server);

  std::vector<OpcMonitor*> controllers;
  for (auto L : loader.getItems()) {
    OpcMonitor* controller;
    if(L.device=="HMP2020"){
      controller=new HMP2020Controller(L.Id);
    }
    else if(L.device=="HMP4040"){
      controller=new HMP4040Controller(L.Id);
    }
    else if(L.device=="DT1415ET"){
      controller=new DT1415Controller(L.Id);
    }
    else{
      std::cout<<"CONFIG: Unknown device "<<L.device<<std::endl;
        continue;
    }
    controllers.push_back(controller);
    controller->init(server.server);
    controller->spawn_thread();
  }

  server.run();

  for (auto i : controllers){
    i->join_thread();
    delete i;
  }
return 0;
}
