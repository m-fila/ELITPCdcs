#include "include/opc/opc_server.h"
#include "include/opc/opc_state.h"
#include "include/opc/hmp2020controller.h"
#include "include/opc/dt1415controller.h"
#include "../common/loader.h"
#include <thread>
int main(int argc, char *argv[])
{
    OpcServer server;

    OpcState state;
    state.init(server.server);




    std::vector<OpcMonitor*> controllers;
    for (auto L : loader::parse("../../dcs.config")) {
   //     for (auto L : loader::parse("dcs.config")) {
        OpcMonitor* controller;
        if(L.device=="HMP2020"){
           controller=new HMP2020Controller(L.Id);
        }
        else if(L.device=="DT1415ET"){
          controller=new DT1415Controller(L.Id);
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
