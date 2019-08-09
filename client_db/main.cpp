#include "include/opc_client.h"
#include <iostream>
#include "include/hmp_variable.h"
#include "include/dt1415_variable.h"
#include "../common/loader.h"
int main(int argc, char *argv[])
{
    opc_client client;
    client.Database.open("test.db");

    for (auto L : loader::parse("../../dcs.config")) {

        if(L.device=="HMP2020"){
           client.addVariable(new hmp_variable(L.Id,"Measurements"));
        }
        else if(L.device=="DT1415ET"){
            client.addVariable(new dt1415_variable(L.Id,"Measurements"));
        }

        else{
           std::cout<<"Unknown device:"<<L.device<<std::endl;
           continue;
        }
        client.addVariable(new status_variable(L.Id,"Status"));
  //  hmp_variable hmp("HMP2","Measurements");
  //  status_variable status("HMP2","Status");
  //  state_variable state("MachineState","State");
    }
    client.run();

    return 0;
}


