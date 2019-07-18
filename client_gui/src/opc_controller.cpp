#include "include/opc_controller.h"

#include <iostream>
opc_controller::opc_controller(std::string OName, QObject *parent):
    opcQObject(OName, parent),
    StatusVariableName(OName+".Status"),
    MeasurementsVariableName(OName+".Measurements"),
    ConfigurationVariableName(OName+".Configuration")
{
}

void opc_controller::opcInit(UA_Client *Client, UA_ClientConfig *Config, UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
    addMonitoredItem(StatusVariableName,response,StatusChangedCallback);
    addMonitoredItem(MeasurementsVariableName,response,MeasurementsChangedCallback);
    addMonitoredItem(ConfigurationVariableName,response,ConfigurationChangedCallback);
}

void opc_controller::StatusChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    void* data=value->value.data;
    opc_controller* context=static_cast<opc_controller*>(monContext);
    emit context->statusChanged(data);
}

void opc_controller::MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    void* data=value->value.data;
    opc_controller* context=static_cast<opc_controller*>(monContext);
    emit context->measurementsChanged(data);

}

void opc_controller::ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    void* data=value->value.data;
    opc_controller* context=static_cast<opc_controller*>(monContext);
    emit context->configurationChanged(data);
}

void opc_controller::callConnect(std::string IPAddress, int port){
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
void opc_controller::callDisconnect(){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("DisconnectDevice"));
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 0, nullptr, nullptr,nullptr);
}

