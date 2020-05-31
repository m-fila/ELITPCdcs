#include "ELITPCServer.h"
#include "DCSState.h"
#include "DCSHMPController.h"
#include "DCSDT1415ETController.h"
#include "DCSTPG362Controller.h"
#include "DCSPiWeatherController.h"
#include "DCSMKS910Controller.h"
#include "DCSN1471Controller.h"
#include "configloader.h"
#include "DCSArt.h"
#include "iostream"
using json = nlohmann::json;
int main(int argc, char *argv[]){
  std::cout<<DCSArt::ascii<<std::endl;
  json config=ConfigLoader::getMasterConfig(argc,argv);
  
  ELITPCServer server(config.at("server").at("address").get<std::string>(),config.at("server").at("port").get<std::string>());
  server.addController<DCSState>("");

  for (auto &i : config.at("devices")){
    auto type=i.at("type").get<std::string>();
    auto id=i.at("id").get<std::string>();
    if(type=="HMP2020"){
      server.addController<DCSHMP2020Controller>(id);
    }
    else if(type=="HMP4040"){
      server.addController<DCSHMP4040Controller>(id);
    }
    else if(type=="DT1415ET"){
      server.addController<DCSDT1415ETController>(id);
    }
    else if(type=="TPG362"){
      server.addController<DCSTPG362Controller>(id);
    }
    else if(type=="PiWeather"){
      server.addController<DCSPiWeatherController>(id);
    }
    else if(type=="MKS910"){
      server.addController<DCSMKS910Controller>(id);
    }
    else if(type=="N1471"){
      server.addController<DCSN1471Controller>(id);
    }
    else{
      std::cout<<"CONFIG: Unknown device "<<type<<std::endl;
        continue;
    }
  }

  server.run();

return 0;

}
