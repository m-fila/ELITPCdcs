#include "include/lvcontroller.h"
#include <iostream>
lv_controller::lv_controller(std::string OName, QObject *parent): opcQObject(OName, parent){
StatusVariableName=ObjectName+".Status";
}

void lv_controller::addMonitoredItem(UA_Client *Client, UA_ClientConfig *Config, UA_CreateSubscriptionResponse response){
           UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"!!!!!!!!!!!!!!!!");
    UA_NodeId StatusId=UA_NODEID_STRING_ALLOC(1,StatusVariableName.c_str());
    client=Client;
    config=Config;
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(StatusId);
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(Client, response.subscriptionId,
                                                                                        UA_TIMESTAMPSTORETURN_BOTH,
                                                                                        monRequest, this, StatusChangedCallback, nullptr);
    if(monResponse.statusCode == UA_STATUSCODE_GOOD){
       UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created Status monitored item");
    }
    UA_NodeId_deleteMembers(&StatusId);
}


void lv_controller::StatusChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    bool isConnected=*static_cast<bool*>(value->value.data);
    lv_controller* context=static_cast<lv_controller*>(monContext);
    emit context->statusChanged(isConnected);
}

void lv_controller::callConnect(std::string IPAddress, int port){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("ConnectDevice"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_String address=UA_String_fromChars(IPAddress.c_str());
    UA_Variant_setScalarCopy(&input[0], &address, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &port, &UA_TYPES[UA_TYPES_INT32]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 2, input, nullptr,nullptr);
    UA_Variant_clear(input);
}
void lv_controller::callDisconnect(){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("DisconnectDevice"));
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 0, nullptr, nullptr,nullptr);
    //std::cout<<UA_StatusCode_name(retval)<<std::endl;
}
