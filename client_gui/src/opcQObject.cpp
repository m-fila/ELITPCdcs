#include "include/opcQObject.h"

opcQObject::opcQObject(std::string OName, QObject *parent) : QObject(parent), ObjectName(OName){
    ObjectNodeId=UA_NODEID_STRING_ALLOC(1,ObjectName.c_str());
}

opcQObject::~opcQObject(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
}

void opcQObject::opcInit(UA_Client *Client, UA_ClientConfig *Config,
                                    UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
}

void opcQObject::addMonitoredItem(std::string VariableName, UA_CreateSubscriptionResponse response,
                                  void (*ValueChangedCallback)(UA_Client *, UA_UInt32, void *, UA_UInt32, void *,
                                                               UA_DataValue *)){

    UA_NodeId VariableId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(VariableId);
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
                                                                                        UA_TIMESTAMPSTORETURN_BOTH,
                                                                                        monRequest, this, ValueChangedCallback, nullptr);
    if(monResponse.statusCode == UA_STATUSCODE_GOOD){
       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created %s monitored item",VariableName.c_str());
    }
    UA_NodeId_deleteMembers(&VariableId);
}
