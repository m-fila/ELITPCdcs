#include "opc_client_gui.h"
//opc_client* opc_client::context=nullptr;

opc_client::opc_client(std::string address, std::string port): 
address(address), 
port(port)
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);
    config->stateCallback=stateCallback;
    config->clientContext=this;
    client_clock=new QTimer;
    client_clock->start(10);
    connectSignals();
}
opc_client::~opc_client(){
    UA_Client_delete(client);
    delete client_clock;
}

void opc_client::iterate(){
    std::string tcp_address="opc.tcp://"+address+":"+port;
    UA_StatusCode retval = UA_Client_connect(client, tcp_address.c_str());
    if(retval==UA_STATUSCODE_GOOD){
    UA_Client_run_iterate(client, 1);
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

void opc_client::addSubscription(UA_Double interval){
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    request.requestedPublishingInterval=interval;
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,nullptr, nullptr, nullptr);
    if(response.responseHeader.serviceResult == UA_STATUSCODE_GOOD){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created subsrciption");
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
