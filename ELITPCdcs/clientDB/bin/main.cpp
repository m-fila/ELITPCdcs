#include "eli_client_db.h"
#include <iostream>
#include "hmp_variable.h"
#include "dt1415_variable.h"
#include "tpg362_variable.h"
#include "piweather_variable.h"
#include "configWrapper.h"
int main(int argc, char *argv[])
{
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

    eli_client client(root["address"].asString(),root["port"].asString());
    
    client.open("test.db");
    for (auto i : root["devices"]) {

        if(i["type"].asString()=="HMP2020"){
           client.addVariable(new HMP2020Variable(i["id"].asString(),"Measurements"));
        }
        else if(i["type"].asString()=="HMP4040"){
           client.addVariable(new HMP4040Variable(i["id"].asString(),"Measurements"));
        }
        else if(i["type"].asString()=="DT1415ET"){
            client.addVariable(new dt1415_variable(i["id"].asString(),"Measurements"));
        }
        else if(i["type"].asString()=="TPG362"){
            client.addVariable(new TPG362Variable(i["id"].asString(),"Measurements"));
        }
        else if(i["type"].asString()=="PiWeather"){
            client.addVariable(new PiWeatherVariable(i["id"].asString(),"Measurements"));
        }
        else{
           std::cout<<"Unknown device:"<<i["type"].asString()<<std::endl;
           continue;
        }
        client.addVariable(new status_variable(i["id"].asString(),"Status"));
    }
    client.addVariable(new state_variable("MachineState","State"));
    client.run();

    return 0;
}


