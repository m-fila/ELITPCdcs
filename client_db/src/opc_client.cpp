#include "../include/opc_client.h"

//opc_client* opc_client::context=nullptr;
UA_Boolean opc_client::running = false;

opc_client::opc_client()
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);
    config->stateCallback=stateCallback;
    config->clientContext=this;
    //context=this;
}

opc_client::~opc_client(){
    UA_Client_delete(client);
}
void opc_client::addVariable(abstract_variable *variable){
    variables.push_back(variable);
    Database.createTable(variable->FullName,variable->translateKeys());
}

void opc_client::addSubscription(){
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,&Database, nullptr, nullptr);
    if(response.responseHeader.serviceResult == UA_STATUSCODE_GOOD){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created subsciption");
        for(auto &i : variables) {
            i->init(client);
            i->addMonitoredItem(client, response);

        //    std::cout<<i->translateName()<<std::endl;
        //    std::cout<<i->translateValue()<<std::endl;
        }
    }
    else{

        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"unable to create subsciption");
    }


}

void opc_client::stateCallback(UA_Client *client, UA_ClientState clientState){
    if(clientState==UA_CLIENTSTATE_SESSION) {
        opc_client* context=static_cast<opc_client*>(UA_Client_getConfig(client)->clientContext);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
        context->addSubscription();
     }
}

int opc_client::run(){
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    running=true;
    while(running) {

            UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
            if(retval != UA_STATUSCODE_GOOD) {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                             "Not connected. Retrying to connect in 1 second");
                /* The connect may timeout after 1 second (see above) or it may fail immediately on network errors */
                /* E.g. name resolution errors or unreachable network. Thus there should be a small sleep here */
                UA_sleep_ms(1000);
                continue;
            }

            UA_Client_run_iterate(client, 1000);
    }
    UA_Client_disconnect(client);
    signal(SIGINT,SIG_DFL);
    signal(SIGTERM,SIG_DFL);

    return EXIT_SUCCESS;
}
void opc_client::addCustomTypes(UA_DataTypeArray *custom){
    config->customDataTypes=custom;
}
