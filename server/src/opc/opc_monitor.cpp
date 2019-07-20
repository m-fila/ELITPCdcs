#include "../../include/opc/opc_monitor.h"
#include <iostream>
opc_monitor::opc_monitor(std::string name): opc_object(name),
//    ObjectName(name),
    MeasurementsVariableName(name+".Measurements"),
    ConfigurationVariableName(name+".Configuration"),
    StatusVariableName(name+".Status")
{//ObjectNodeId=UA_NODEID_STRING_ALLOC(1,name.c_str());
}

opc_monitor::~opc_monitor(){
        UA_NodeId_deleteMembers(&ObjectNodeId);
}

void opc_monitor::addMeasurementsVariable(UA_Server *server){
    addVariable(server,MeasurementsVariableName,VariableTypeM,VariableTypeM.typeId);
}
void opc_monitor::addConfigurationVariable(UA_Server *server){
    addVariable(server,ConfigurationVariableName,VariableTypeC,VariableTypeC.typeId);
}
void opc_monitor::addStatusVariable(UA_Server *server){
    addVariable(server,StatusVariableName,UA_TYPES[UA_TYPES_BOOLEAN],UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE) );
}


void opc_monitor::MeasurementsReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        opc_monitor* Monitor=static_cast<opc_monitor*>(nodeContext);
        Monitor->updateMeasurements(server);
        }
void opc_monitor::ConfigurationReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        opc_monitor* Monitor=static_cast<opc_monitor*>(nodeContext);
        Monitor->updateConfiguration(server);
        }
void opc_monitor::StatusReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        opc_monitor* Monitor=static_cast<opc_monitor*>(nodeContext);
        Monitor->updateStatus(server);
        }



void opc_monitor::addValueCallback(UA_Server *server,std::string VariableName,  void (*ReadCallback)(UA_Server *server, const UA_NodeId *sessionId,
                                                                                               void *sessionContext, const UA_NodeId *nodeid,
                                                                                               void *nodeContext, const UA_NumericRange *range,
                                                                            const UA_DataValue *value)) {
        UA_NodeId currentNodeId = UA_NODEID_STRING_ALLOC(1, VariableName.c_str());
        UA_ValueCallback callback;
        callback.onRead = ReadCallback;
        callback.onWrite= nullptr;
        UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);
        UA_NodeId_deleteMembers(&currentNodeId);
        }

UA_StatusCode opc_monitor::DisconnectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
   // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Disconnect was called");
    opc_monitor* Monitor=static_cast<opc_monitor*>(methodContext);
    Monitor->disconnectDevice();
    return UA_STATUSCODE_GOOD;
}

void opc_monitor::addDisconnectDeviceMethod(UA_Server *server) {
    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Disconnect device");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","disconnect");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"DisconnectDevice");
    UA_QualifiedName MethodQName=UA_QUALIFIEDNAME_ALLOC(1,"disconnect");
    UA_Server_addMethodNode(server, MethodNodeId,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &DisconnectDeviceCallback,
                            0, nullptr, 0, nullptr, this, nullptr);
    UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
    UA_MethodAttributes_deleteMembers(&methodAttr);
}

UA_StatusCode opc_monitor::ConnectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
   // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Connect was called");

    opc_monitor* Monitor=static_cast<opc_monitor*>(methodContext);
    UA_Int32 port = *(UA_Int32*)input[1].data;
    UA_String host = *(UA_String*)input[0].data;
    std::string address(reinterpret_cast<char*>(host.data));


    TCPConnectionParameters parameters;
    parameters.IPaddress=address;
    parameters.port=port;
    Monitor->connectDevice(&parameters);
    return UA_STATUSCODE_GOOD;
}
void opc_monitor::addConnectDeviceMethod(UA_Server *server) {

    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Host");
    inputArguments[0].name = UA_String_fromChars("Host address");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Port");
    inputArguments[1].name = UA_String_fromChars("TCP port");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Connect device");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","connect");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"ConnectDevice");
    UA_QualifiedName MethodQName=UA_QUALIFIEDNAME_ALLOC(1,"connect");
    UA_Server_addMethodNode(server, MethodNodeId,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &ConnectDeviceCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
}



void opc_monitor::addMonitoredItem(UA_Server *server,std::string VariableName,UA_Double sampling) {
    UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, VariableName.c_str());
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(NodeId);
    monRequest.requestedParameters.samplingInterval = sampling; /* 100 ms interval */
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, nullptr, dataChangeNotificationCallback);
    UA_NodeId_deleteMembers(&NodeId);
}


void opc_monitor::dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                              void *monitoredItemContext, const UA_NodeId *nodeId,
                              void *nodeContext, UA_UInt32 attributeId,
                              const UA_DataValue *value) {
   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Notification");
}
