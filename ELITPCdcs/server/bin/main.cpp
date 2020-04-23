#include "eli_server.h"
#include "opc_state.h"
#include "hmp2020controller.h"
#include "dt1415controller.h"
#include "tpg362controller.h"
#include "piweathercontroller.h"
#include "mks910controller.h"
#include "n1471controller.h"
#include <memory>
#include <fstream>
#include "configloader.h"
using json = nlohmann::json;
int main(int argc, char *argv[]){

  json config=ConfigLoader::getMasterConfig(argc,argv);

  ELIServer server(config.at("address").get<std::string>(),config.at("port").get<std::string>());
  OpcState state;
  state.init(server.server);

  for (auto &i : config.at("devices")){
    auto type=i.at("type").get<std::string>();
    auto id=i.at("id").get<std::string>();
    if(type=="HMP2020"){
      server.addController<HMP2020Controller>(id);
    }
    else if(type=="HMP4040"){
      server.addController<HMP4040Controller>(id);
    }
    else if(type=="DT1415ET"){
      server.addController<DT1415Controller>(id);
    }
    else if(type=="TPG362"){
      server.addController<TPG362Controller>(id);
    }
    else if(type=="PiWeather"){
      server.addController<PiWeatherController>(id);
    }
    else if(type=="MKS910"){
      server.addController<MKS910Controller>(id);
    }
    else if(type=="N1471"){
      server.addController<N1471Controller>(id);
    }
    else{
      std::cout<<"CONFIG: Unknown device "<<type<<std::endl;
        continue;
    }
  }

  server.run();

return 0;

}
