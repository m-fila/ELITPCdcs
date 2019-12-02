#include "opc_client.h"
#include <iostream>
#include "hmp_variable.h"
#include "dt1415_variable.h"
#include "tpg362_variable.h"
#include "piweather_variable.h"
#include "loader.h"
#include "configWrapper.h"
int main(int argc, char *argv[])
{
  /*Loader loader(argc, argv);
  if(loader.isOpened()){
    loader.parse();
  }
  else{
    std::cout<<"Can't open config file"<<std::endl;
    return 0;
  }*/
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

    opc_client client(root["address"].asString(),root["port"].asString());
    
    client.Database.open("test.db");
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


