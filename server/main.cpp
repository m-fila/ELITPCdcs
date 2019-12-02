#include "opc/opc_server.h"
#include "opc/opc_state.h"
#include "opc/hmp2020controller.h"
#include "opc/dt1415controller.h"
#include "opc/tpg362controller.h"
#include "opc/piweathercontroller.h"
#include "configWrapper.h"
#include <memory>

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

  OpcServer server(root["address"].asString(),root["port"].asString());
  OpcState state;
  state.init(server.server);

  std::vector<std::unique_ptr<OpcMonitor>> controllers;
  for (auto i : root["devices"]){
    std::unique_ptr<OpcMonitor> controller;
    if(i["type"].asString()=="HMP2020"){
      controller=std::unique_ptr<HMP2020Controller>(new HMP2020Controller(i["id"].asString()));
    }
    else if(i["type"].asString()=="HMP4040"){
      controller=std::unique_ptr<HMP4040Controller>(new HMP4040Controller(i["id"].asString()));
    }
    else if(i["type"].asString()=="DT1415ET"){
      controller=std::unique_ptr<DT1415Controller>(new DT1415Controller(i["id"].asString()));
    }
    else if(i["type"].asString()=="TPG362"){
      controller=std::unique_ptr<TPG362Controller>(new TPG362Controller(i["id"].asString()));
    }
    else if(i["type"].asString()=="PiWeather"){
      controller=std::unique_ptr<PiWeatherController>(new PiWeatherController(i["id"].asString()));
    }
    else{
      std::cout<<"CONFIG: Unknown device "<<i["type"].asString()<<std::endl;
        continue;
    }

    controller->init(server.server);
    controllers.push_back(std::move(controller));
  }

  server.run();

return 0;

}
