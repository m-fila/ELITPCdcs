#include "include/state.h"
#include <iostream>
stateMachine::stateMachine( std::string OName,QObject *parent): opcQObject(OName,parent){
    std::string VariableName=ObjectName+".State";
    VariableNodeId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    MethodNodeId =UA_NODEID_STRING(1,const_cast<char*>("SetState"));
}

stateMachine::~stateMachine(){
    UA_NodeId_deleteMembers(&VariableNodeId);
}

void stateMachine::requestChange(int index){
std::cout<<index<<std::endl;

    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &index, &UA_TYPES[UA_TYPES_INT32]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 1, &input, nullptr,nullptr);
   std::cout<<UA_StatusCode_name(retval)<<std::endl;
    UA_Variant_clear(&input);
}

void stateMachine::valueChanged_callback(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value){
    int index=*static_cast<int*>(value->value.data);
    stateMachine* context=static_cast<stateMachine*>(monContext);
    emit context->stateChanged(index);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"State value updated");

}

void stateMachine::addMonitoredItem(UA_Client *Client, UA_ClientConfig *Config,
                                    UA_CreateSubscriptionResponse response){

    client=Client;
    config=Config;
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(VariableNodeId);
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(Client, response.subscriptionId,
                                                                                        UA_TIMESTAMPSTORETURN_BOTH,
                                                                                        monRequest, this, valueChanged_callback, nullptr);
    if(monResponse.statusCode == UA_STATUSCODE_GOOD){
       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created State monitored item");
    }

}
