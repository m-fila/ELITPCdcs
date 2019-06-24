#include "../include/abstract_variable.h"

abstract_variable::abstract_variable(std::string name) : VariableName(name)
{
    VariableId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
}
abstract_variable::~abstract_variable(){
    UA_NodeId_deleteMembers(&VariableId);
}

void abstract_variable::handler_ValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                                             UA_UInt32 monId, void *monContext, UA_DataValue *value){
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"notification!");
    abstract_variable *variable=static_cast<abstract_variable*>(monContext);
    variable->updateData(value);
}

void abstract_variable::addMonitoredItem(UA_Client *client,UA_CreateSubscriptionResponse response){
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(VariableId);
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
                                                                                         UA_TIMESTAMPSTORETURN_BOTH,
                                                                                         monRequest, this, handler_ValueChanged, NULL);
    if(monResponse.statusCode == UA_STATUSCODE_GOOD){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created monitored item");
    }
}
