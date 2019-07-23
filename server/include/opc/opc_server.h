#ifndef OPC_SERVER_H
#define OPC_SERVER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>

class opc_server
{
public:
    opc_server();
    ~opc_server();
    UA_Server* server;
    UA_ServerConfig * config;
    int run();
    void addCustomTypes(UA_DataTypeArray *custom);


private:    
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }
    static void Scan(UA_Server* server, void *data);
};

#endif // OPC_SERVER_H
