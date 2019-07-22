#include "include/opc_client.h"
//opc_client* opc_client::context=nullptr;
opc_client::opc_client()
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);
    config->stateCallback=stateCallback;
    config->clientContext=this;
    client_clock=new QTimer;
    client_clock->start(100);
   // context=this;

   // hmp_customType hmp_customType;
    //UA_DataTypeArray* hmpCustom=
    addCustomTypes(&HMPArray);
    connectSignals();

}
opc_client::~opc_client(){
    UA_Client_delete(client);
    delete client_clock;
}

void opc_client::iterate(){
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6666");
    if(retval==UA_STATUSCODE_GOOD){
    UA_Client_run_iterate(client, 10);
    }
}
void opc_client::connectSignals(){
    connect(client_clock,SIGNAL(timeout()),this,SLOT(iterate()));
}

void opc_client::stateCallback (UA_Client *client, UA_ClientState clientState){
    if(clientState==UA_CLIENTSTATE_SESSION) {
        opc_client* context=static_cast<opc_client*>(UA_Client_getConfig(client)->clientContext);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
        context->addSubscription();
    }
//    if(clientState==UA_CLIENTSTATE_DISCONNECTED){
//        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A connection with the server is closed");
 //   }
}

void opc_client::addSubscription(){
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,nullptr, nullptr, nullptr);
    if(response.responseHeader.serviceResult == UA_STATUSCODE_GOOD){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created subsciption");
        emit subCreated(client, config, response);
    }
    else
    {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"failed to create sub");
    }
}

void opc_client::addCustomTypes(UA_DataTypeArray *custom){
    config->customDataTypes=custom;
}
