#include "include/database.h"
#include "include/opc_client.h"
#include <iostream>
#include "include/hmp_variable.h"
int main(int argc, char *argv[])
{
//    if(client.init()){
//        return 1;
//    }
   opc_client client;
   //client.go();
   hmp_variable hmp("HMP2.Measurements");
   status_variable status("HMP2.Status");
   state_variable state("MachineState.State");

   client.addVariable(&hmp);
   client.addVariable(&status);
   client.addVariable(&state);
   client.addSubscription();
   client.run();
    return 0;
}
