#include "include/opc_client.h"
#include <iostream>
#include "include/hmp_variable.h"
#include "include/dt1415_variable.h"
#include "include/tpg362_variable.h"
#include "../common/loader.h"
int main(int argc, char *argv[])
{
  Loader loader(argc, argv);
  if(loader.isOpened()){
    loader.parse();
  }
  else{
    std::cout<<"Can't open config file"<<std::endl;
    return 0;
  }

    opc_client client(loader.getAddress(),loader.getPort());
    
    client.Database.open("test.db");
    for (auto L : loader.getItems()) {

        if(L.device=="HMP2020"){
           client.addVariable(new HMP2020Variable(L.Id,"Measurements"));
        }
        else if(L.device=="HMP4040"){
           client.addVariable(new HMP4040Variable(L.Id,"Measurements"));
        }
        else if(L.device=="DT1415ET"){
            client.addVariable(new dt1415_variable(L.Id,"Measurements"));
        }
        else if(L.device=="TPG362"){
            client.addVariable(new TPG362Variable(L.Id,"Measurements"));
        }

        else{
           std::cout<<"Unknown device:"<<L.device<<std::endl;
           continue;
        }
        client.addVariable(new status_variable(L.Id,"Status"));
    }
    client.addVariable(new state_variable("MachineState","State"));
    client.run();

    return 0;
}


