#include "include/opc/opc_server.h"
#include "include/opc/opc_state.h"
#include "include/opc/hmpcontroller.h"
#include "include/opc/dtcontroller.h"
#include "../common/loader.h"

int main(int argc, char *argv[])
{
    opc_server server;

    opc_state state;
    state.init(server.server);
  //  UA_DataTypeArray hmpcustom=controller.customType.DataTypeArray(nullptr); //{nullptr,1,types};
//    server.addCustomTypes(&hmpcustom);


    std::vector<opc_monitor*> controllers;
    for (auto L : loader::parse("../dcs_master/dcs.config")) {
        opc_monitor* controller;
        if(L.device=="HMP2020"){
           controller=new HMPController(L.Id);
        }
        else if(L.device=="DT1415ET"){
          controller=new DTController(L.Id);
        }
        else{
           std::cout<<"CONFIG: Unknown device "<<L.device<<std::endl;
           continue;
        }
        controllers.push_back(controller);
        controller->init(server.server);
    }
    server.run();

    for (auto i : controllers)
        delete i;
return 0;
}
