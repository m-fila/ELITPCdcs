#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include "HMPDataType.h"
#include "abstract_variable.h"
#include "hmp_variable.h"

#include <vector>
#include <iostream>
class opc_client
{
public:
    UA_Client *client;
    UA_ClientConfig *config;
    opc_client();
    ~opc_client();
    int init();
    void go();
//    void addDataTypes();
    std::vector<abstract_variable* > variables;
};

#endif // OPC_CLIENT_H
