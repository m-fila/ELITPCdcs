#ifndef OPC_SERVER_H
#define OPC_SERVER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <string>
#include "open62541/namespace_dcsnodeset_generated.h"


class OpcServer
{
public:
    OpcServer(std::string address, std::string port);
    ~OpcServer();
    UA_Server* server;
    UA_ServerConfig * config;
    int run();
    void addCustomTypes(UA_DataTypeArray *custom);
    void addNamespace();
    UA_DataTypeArray customDataTypesArray =
    { nullptr, UA_TYPES_DCSNODESET_COUNT, UA_TYPES_DCSNODESET};
private:    
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }
};

#endif // OPC_SERVER_H
