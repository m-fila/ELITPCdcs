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
class opc_client
{
public:
    opc_client();
    ~opc_client();
    UA_Client *client;
    UA_ClientConfig *config;

static    opc_client* ptr;
    void addSubscription();
    void addMonitoredItemstoSubsription();

    bool init();
    void go();
    int run();
//    void addDataTypes();
    std::vector<std::unique_ptr<abstract_variable>> variables;


static void stateCallback (UA_Client *client, UA_ClientState clientState);
private:

    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c. Shutting down may take a few seconds");
        running = false;
    }



};

#endif // OPC_CLIENT_H
