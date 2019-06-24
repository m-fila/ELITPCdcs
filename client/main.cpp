#include "include/database.h"
#include "include/opc_client.h"
#include <iostream>
int main(int argc, char *argv[])
{
    opc_client client;
//    if(client.init()){
//        return 1;
//    }
   client.go();
   //client.run();
    return 0;
}
