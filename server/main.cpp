#include "include/opc/opc_server.h"
#include "include/opc/opc_state.h"
#include "include/opc/hmp2020controller.h"
#include "include/opc/dt1415controller.h"
#include "include/opc/tpg362controller.h"
#include "include/opc/piweathercontroller.h"
#include "../common/loader.h"

int main(int argc, char *argv[]){
/*
GenericDevice a;
a.setConnectionStream(TCPConnector::connect("192.168.168.40",8000));
//a.sendCommand("AYT\r");
std::string r=a.sendWithResponse("PRX\r");
std::cout<<int(r.at(0))<<std::endl;
const std::string s(1,5);
std::cout<<a.sendWithResponse(s)<<std::endl;
return 0;
*/
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
    else if(L.device=="TPG362"){
      controller=new TPG362Controller(L.Id);
    }
    else if(L.device=="PiWeather"){
      controller=new PiWeatherController(L.Id);
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
