#include "include/lvcontroller.h"
#include <iostream>
lv_controller::lv_controller(std::string OName, QObject *parent):
    opcQObject(OName, parent),
    StatusVariableName(OName+".Status"),
    MeasurementsVariableName("HMP2.Measurements"),
    ConfigurationVariableName(OName+".Configuration")
{
}

void lv_controller::opcInit(UA_Client *Client, UA_ClientConfig *Config, UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
    addMonitoredItem(StatusVariableName,response,StatusChangedCallback);
    addMonitoredItem(MeasurementsVariableName,response,MeasurementsChangedCallback);
    addMonitoredItem(ConfigurationVariableName,response,ConfigurationChangedCallback);
}

void lv_controller::StatusChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    bool isConnected=*static_cast<bool*>(value->value.data);
    lv_controller* context=static_cast<lv_controller*>(monContext);
    emit context->statusChanged(isConnected);
}

void lv_controller::MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    HMPMeasurements measurements=*static_cast<HMPMeasurements*>(value->value.data);
    lv_controller* context=static_cast<lv_controller*>(monContext);
    emit context->measurementsChanged(measurements);

}

void lv_controller::ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                            UA_UInt32 monId, void *monContext, UA_DataValue *value){
    HMPMeasurements configuration=*static_cast<HMPMeasurements*>(value->value.data);
    lv_controller* context=static_cast<lv_controller*>(monContext);
    emit context->configurationChanged(configuration);
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
}

void lv_controller::callSetOutput(bool state){
       UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input;
       UA_Variant_init(&input);
       UA_Variant_setScalarCopy(&input, &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   MethodNodeId, 1, &input, nullptr,nullptr);
       UA_Variant_clear(&input);
}

void lv_controller::callSetChannel(int nr, bool state){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 2, input, nullptr,nullptr);
    UA_Variant_clear(input);
}
