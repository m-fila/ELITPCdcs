#ifndef OPC_CLIENT_H
#define OPC_CLIENT_H

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
//#include "HMPDataType.h"
#include "abstract_variable.h"
#include "hmp_variable.h"

#include <vector>
#include <iostream>
class opc_client
{
public:
    opc_client();
    ~opc_client();
    UA_Client *client;
    UA_ClientConfig *config;

    bool init();
    void go();
    int run();
//    void addDataTypes();
    std::vector<abstract_variable* > variables;

private:

    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }



};

#endif // OPC_CLIENT_H
