#include "opc_client_gui.h"

//opc_client* opc_client::context=nullptr;

opc_client::opc_client(std::string address, int port): 
tcp_address("opc.tcp://"+address+":"+std::to_string(port))
{
    client = UA_Client_new();
    config= UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(config);
    config->stateCallback=stateCallback;
    config->clientContext=this;
    client_clock=new QTimer(this);
    connectSignals();
}
opc_client::~opc_client(){
    client_clock->stop();
    UA_Client_delete(client);
}

void opc_client::iterate(){
    if(connection!=Connection::Opened){
        UA_Client_connect(client, tcp_address.c_str());
    }
    if(connection==Connection::Opened){
    UA_Client_run_iterate(client, 5);
    }
}

void opc_client::connectSignals(){
    connect(client_clock,SIGNAL(timeout()),this,SLOT(iterate()));
}

void
opc_client::stateCallback(UA_Client *client, UA_SecureChannelState channelState,
              UA_SessionState sessionState, UA_StatusCode recoveryStatus){
    opc_client* context=static_cast<opc_client*>(UA_Client_getConfig(client)->clientContext);
    if (sessionState == UA_SESSIONSTATE_ACTIVATED) {
        
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
        context->addSubscription();
    }
    if(channelState==UA_SECURECHANNELSTATE_OPEN && context->connection!=Connection::Opened){
        context->connection=Connection::Opened;
        emit context->clientConnected(true);
        context->client_clock->setInterval(10);
        
    }
    else if(channelState==UA_SECURECHANNELSTATE_CLOSED && context->connection!=Connection::Closed){
        context->connection=Connection::Closed;
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A connection with the server is closed");
        context->client_clock->setInterval(1000);
        emit context->clientConnected(false);
    }
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

