#include "eli_client_db.h"

#include "hmp_variable.h"
#include "dt1415_variable.h"
#include "tpg362_variable.h"
#include "piweather_variable.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
int main(int argc, char *argv[])
{
  json config;
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
    ifs>>config;
  }
  else{
    std::cout<<"Can't find config file"<<std::endl;
    return 0;
  }

    eli_client client(config.at("address").get<std::string>(),config.at("port").get<std::string>());
    
    client.open("test.db");
    for (auto & i : config.at("devices")) {
        auto type=i.at("type").get<std::string>();
        auto id=i.at("id").get<std::string>();
        if(type=="HMP2020"){
           client.addVariable(new HMP2020Variable(id,"Measurements"));
        }
        else if(type=="HMP4040"){
           client.addVariable(new HMP4040Variable(id,"Measurements"));
        }
        else if(type=="DT1415ET"){
            client.addVariable(new dt1415_variable(id,"Measurements"));
        }
        else if(type=="TPG362"){
            client.addVariable(new TPG362Variable(id,"Measurements"));
        }
        else if(type=="PiWeather"){
            client.addVariable(new PiWeatherVariable(id,"Measurements"));
        }
        else{
           std::cout<<"Unknown device:"<<type<<std::endl;
           continue;
        }
        client.addVariable(new status_variable(id,"Status"));
    }
    client.addVariable(new state_variable("MachineState","State"));
    client.run();

    return 0;
}


