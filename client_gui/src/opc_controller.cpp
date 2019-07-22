#include "include/opc_controller.h"

#include <iostream>
opc_controller::opc_controller(std::string OName, QObject *parent):
    opcQObject(OName, parent),
    StatusVariableName("Status"),
    MeasurementsVariableName("Measurements"),
    ConfigurationVariableName("Configuration")
{
}

void opc_controller::browseIds(){
    std::string connectBrowseName="connect";
    std::string disconnectBrowseName="disconnect";
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    bReq.nodesToBrowse[0].nodeId =ObjectNodeId;// UA_NODEID_NUMERIC(0, UA_NS0ID_OBJE
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_BROWSENAME;
    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    for(size_t i = 0; i < bResp.resultsSize; ++i) {
            for(size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription ref = (bResp.results[i].references[j]);
                std::string str =std::string(reinterpret_cast<char*>(ref.browseName.name.data));
                if(!str.compare(MeasurementsVariableName))
                    MeasurementsNodeId=ref.nodeId.nodeId;
                if(!str.compare(ConfigurationVariableName))
                    ConfigurationNodeId=ref.nodeId.nodeId;
                if(!str.compare(StatusVariableName))
                    StatusNodeId=ref.nodeId.nodeId;
                if(!str.compare(connectBrowseName))
                    connectNodeId=ref.nodeId.nodeId;
                if(!str.compare(disconnectBrowseName))
                    disconnectNodeId=ref.nodeId.nodeId;
            }
    }
}

void opc_controller::opcInit(UA_Client *Client, UA_ClientConfig *Config, UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
    browseIds();
    addMonitoredItem(StatusNodeId,response,StatusChangedCallback);
    addMonitoredItem(MeasurementsNodeId,response,MeasurementsChangedCallback);
    addMonitoredItem(ConfigurationNodeId,response,ConfigurationChangedCallback);
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
                                connectNodeId, 2, input, nullptr,nullptr);
    UA_Variant_clear(input);
}
void opc_controller::callDisconnect(){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("DisconnectDevice"));
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                disconnectNodeId, 0, nullptr, nullptr,nullptr);
}

