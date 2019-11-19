﻿#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <vector>
#include <iostream>
#include <memory>
#include "abstract_variable.h"
//#include "hmp_variable.h"
#include "database.h"
#include "open62541/types_dcsnodeset_generated.h"
class opc_client
{
public:
    opc_client(std::string address,std::string port);
    ~opc_client();
    opc_client(opc_client const&) = delete;
    void operator=(opc_client const&)  = delete;

    UA_Client *client;
    UA_ClientConfig *config;
    database Database;

    void addSubscription();
    void addVariable(abstract_variable *variable);
    void addCustomTypes(UA_DataTypeArray *custom);
    int run();
private:
    std::string address;
    std::string port;
    UA_DataTypeArray customDataTypesArray = { nullptr, UA_TYPES_DCSNODESET_COUNT, UA_TYPES_DCSNODESET};
    std::vector<abstract_variable*> variables;
    static void stateCallback (UA_Client *client, UA_ClientState clientState);
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c. Shutting down may take a few seconds");
        running = false;
    }

};

#endif // OPC_CLIENT_H