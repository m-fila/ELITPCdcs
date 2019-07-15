#include "include/state.h"
#include <iostream>
stateMachine::stateMachine()
{
ObjectName="MachineState";
VariableName=ObjectName+".State";
}

void stateMachine::requestChange(int index){
std::cout<<index<<std::endl;

    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &index, &UA_TYPES[UA_TYPES_INT32]);
    UA_StatusCode retval= UA_Client_call(client, UA_NODEID_STRING_ALLOC(1,ObjectName.c_str()),
                                UA_NODEID_STRING_ALLOC(1,"SetState"), 1, &input, nullptr,nullptr);
   std::cout<<UA_StatusCode_name(retval)<<std::endl;
    UA_Variant_clear(&input);
}

void stateMachine::valueChanged_callback(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value){
    int index=*static_cast<int*>(value->value.data);
    stateMachine* context=static_cast<stateMachine*>(monContext);
    emit context->stateChanged(index);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"State value updated");

}

void stateMachine::addMonitoredItem(UA_CreateSubscriptionResponse response){
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING_ALLOC(1,VariableName.c_str()));
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
                                                                                        UA_TIMESTAMPSTORETURN_BOTH,
                                                                                        monRequest, this, valueChanged_callback, nullptr);
    if(monResponse.statusCode == UA_STATUSCODE_GOOD){
       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created State monitored item");
    }

}
