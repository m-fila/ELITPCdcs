#include "include/opc/opc_server.h"
#include "include/opc/opc_state.h"
#include "include/opc/hmpcontroller.h"
#include "include/opc/dtcontroller.h"
#include "../common/loader.h"
#include <thread>
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
    server.addCustomTypes(&hmpcustom);


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

       // std::thread tt([](){ std::cout<<"Thread!"<<std::endl; });
       // std::thread thread_object( [i] { i->update(); } );
    for (auto i : controllers){
        i->spawn_thread();
    }
    server.run();
    for (auto i : controllers){
        i->join_thread();
        delete i;
    }
return 0;
}
