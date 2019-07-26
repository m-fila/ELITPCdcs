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

    hmp_customType hmp;
    dtm_customType dtm;
    dtc_customType dtc;

    UA_DataTypeArray dtmcustom=dtm.DataTypeArray(nullptr);
    UA_DataTypeArray dtccustom=dtc.DataTypeArray(&dtmcustom);
    UA_DataTypeArray hmpcustom=hmp.DataTypeArray(&dtccustom);//{nullptr,1,types};
    server.addCustomTypes(&dtccustom);


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
