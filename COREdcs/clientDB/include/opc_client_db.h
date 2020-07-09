#ifndef OPC_CLIENT_DB_H
#define OPC_CLIENT_DB_H

#include "abstract_variable.h"
#include <iostream>
#include <memory>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>
#include <vector>
//#include "hmp_variable.h"
#include "database.h"

class opc_client_db {
  public:
    opc_client_db(std::string address, int port);
    ~opc_client_db();
    opc_client_db(opc_client_db const &) = delete;
    void operator=(opc_client_db const &) = delete;

    UA_Client *client;
    UA_ClientConfig *config;

    void addSubscription();
    void addVariable(abstract_variable *variable);
    void addCustomTypes(UA_DataTypeArray *custom);
    int run();
    void open(std::string dbname);

  protected:
    std::string address;
    int port;
    std::vector<abstract_variable *> variables;
    database dbase;
    static void stateCallback(UA_Client *client, UA_SecureChannelState channelState,
                              UA_SessionState sessionState, UA_StatusCode recoveryStatus);
    static UA_Boolean running;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "received ctrl-c. Shutting down may take a few seconds");
        running = false;
    }
};

#endif  // OPC_CLIENT_DB_H
