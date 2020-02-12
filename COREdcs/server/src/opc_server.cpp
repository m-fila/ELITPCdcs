#include "opc_server.h"
//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
//#include <open62541/server_config_default.h>


OpcServer::OpcServer(std::string address, std::string port)
{
    server=UA_Server_new();
    config=UA_Server_getConfig(server);
    //UA_ServerConfig_setDefault(config);
    UA_ServerConfig_setMinimal(config, std::stoi(port), nullptr);
     UA_String hostname= UA_STRING_ALLOC(address.c_str());
    UA_ServerConfig_setCustomHostname(config,hostname);
    UA_String_deleteMembers(&hostname);
    //UA_Server_addRepeatedCallback(server, Scan, nullptr, 100, NULL);
}

OpcServer::~OpcServer(){
    UA_Server_delete(server);
}

void OpcServer::addCustomTypes(UA_DataTypeArray *custom){
    config->customDataTypes=custom;
}

UA_Boolean OpcServer::running = false;

int OpcServer::run(){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    running=true;
    UA_Server_run(server,&running);
    signal(SIGINT,SIG_DFL);
    signal(SIGTERM,SIG_DFL);
    return EXIT_SUCCESS;
}

