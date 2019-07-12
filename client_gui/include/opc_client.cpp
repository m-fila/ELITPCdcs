#include "opc_client.h"
#include <iostream>
opc_client::opc_client(QObject *parent) : QObject(parent)
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);
}
opc_client::~opc_client(){
    UA_Client_delete(client);
}
void opc_client::run(){
    std::cout<<"AAAAAAAAAAAAAAAAAAAA"<<std::endl;
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
}
