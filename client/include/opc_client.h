#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <vector>
#include <iostream>
#include <memory>
#include "abstract_variable.h"
#include "hmp_variable.h"
#include "database.h"
class opc_client //singleton
{
public:
    opc_client();
    ~opc_client();
    opc_client(opc_client const&) = delete;
    void operator=(opc_client const&)  = delete;

    UA_Client *client;
    UA_ClientConfig *config;
    static opc_client* context;
    database Database;


    void addSubscription();

    void addVariable(abstract_variable *variable);
    void go();

    void addCustomTypes(UA_DataTypeArray *custom);
    void addTimedCallback();
    int run();
private:
    std::vector<abstract_variable*> variables;
    static void stateCallback (UA_Client *client, UA_ClientState clientState);
    static void DatabaseTimedCallback(UA_Client *client, void *context);
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c. Shutting down may take a few seconds");
        running = false;
    }

};

#endif // OPC_CLIENT_H
