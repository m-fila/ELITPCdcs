#include "eli_server.h"
#include "opc_state.h"
#include "hmp2020controller.h"
#include "dt1415controller.h"
#include "tpg362controller.h"
#include "piweathercontroller.h"
#include "mks910controller.h"
#include <memory>
#include <fstream>
#include "configloader.h"
using json = nlohmann::json;
int main(int argc, char *argv[]){

  json config=ConfigLoader::getMasterConfig(argc,argv);

  ELIServer server(config.at("address").get<std::string>(),config.at("port").get<std::string>());
  OpcState state;
  state.init(server.server);

  std::vector<std::unique_ptr<OpcMonitor>> controllers;
  for (auto &i : config.at("devices")){
    std::unique_ptr<OpcMonitor> controller;
    auto type=i.at("type").get<std::string>();
    auto id=i.at("id").get<std::string>();
    if(type=="HMP2020"){
      controller=std::unique_ptr<HMP2020Controller>(new HMP2020Controller(id));
    }
    else if(type=="HMP4040"){
      controller=std::unique_ptr<HMP4040Controller>(new HMP4040Controller(id));
    }
    else if(type=="DT1415ET"){
      controller=std::unique_ptr<DT1415Controller>(new DT1415Controller(id));
    }
    else if(type=="TPG362"){
      controller=std::unique_ptr<TPG362Controller>(new TPG362Controller(id));
    }
    else if(type=="PiWeather"){
      controller=std::unique_ptr<PiWeatherController>(new PiWeatherController(id));
    }
    else if(type=="MKS910"){
      controller=std::unique_ptr<MKS910Controller>(new MKS910Controller(id));
    }
    else{
      std::cout<<"CONFIG: Unknown device "<<type<<std::endl;
        continue;
    }
    controller->init(server.server);
    controllers.push_back(std::move(controller));
  }

  server.run();

return 0;

}
