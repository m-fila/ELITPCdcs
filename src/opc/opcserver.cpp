#include "opcserver.h"
//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
//#include <open62541/server_config_default.h>





opcserver::opcserver()
{
    server=UA_Server_new();
    config=UA_Server_getConfig(server);
    //UA_ServerConfig_setDefault(config);
    UA_ServerConfig_setMinimal(config, 6666, nullptr);
    const UA_String hostname= UA_STRING(const_cast<char*>("127.0.0.1"));
    UA_ServerConfig_setCustomHostname(config,hostname);

    //UA_Server_addRepeatedCallback(server, Scan, nullptr, 100, NULL);

}
opcserver::~opcserver(){
   // config->customHostname = UA_STRING_NULL;
  //  UA_ServerConfig_clean(config);
    UA_Server_delete(server);
}

void opcserver::Scan(UA_Server* server, void *data){
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "YO!");}

UA_Boolean opcserver::running = false;

int opcserver::run(){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    running=true;
    UA_Server_run(server,&running);
    signal(SIGINT,SIG_DFL);
    signal(SIGTERM,SIG_DFL);
    return EXIT_SUCCESS;
}

