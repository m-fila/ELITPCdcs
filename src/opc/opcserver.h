#ifndef OPCSERVER_H
#define OPCSERVER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <signal.h>

class opcserver
{
public:
    opcserver();
    ~opcserver();
    UA_Server* server;
    UA_ServerConfig * config;
    int run();

private:    
    void addTypesandVariables();
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }
    static void Scan(UA_Server* server, void *data);
};

#endif // OPCSERVER_H
