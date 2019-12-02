#include "../../include/opc/opc_monitor.h"
#include <iostream>
OpcMonitor::OpcMonitor(std::string name): OpcObject(name), thread_running(false),
    measurementsVariableName("Measurements"),
    configurationVariableName("Configuration"),
    statusVariableName("Status")
    {
}

OpcMonitor::~OpcMonitor(){
    if (thread_running){
        join_thread();
    }
        UA_NodeId_deleteMembers(&objectNodeId);
}

void OpcMonitor::addMeasurementsVariable(UA_Server *server){
    addVariable3(server,&measurementsId,measurementsVariableName,variableTypeM);
    addValueCallback(server,measurementsId,measurementsReadCallback);
}

void OpcMonitor::addConfigurationVariable(UA_Server *server){
    addVariable3(server,&configurationId,configurationVariableName,variableTypeC);
    addValueCallback(server,configurationId ,configurationReadCallback);
}

void OpcMonitor::addStatusVariable(UA_Server *server){
    addVariable3(server,&statusId,statusVariableName,UA_TYPES[UA_TYPES_BOOLEAN]);
    addValueCallback(server,statusId ,statusReadCallback);
}


void OpcMonitor::measurementsReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        OpcMonitor* monitor=static_cast<OpcMonitor*>(nodeContext);
        monitor->updateMeasurementsVariable(server);
        }

void OpcMonitor::configurationReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        OpcMonitor* monitor=static_cast<OpcMonitor*>(nodeContext);
        monitor->updateConfigurationVariable(server);
        }

void OpcMonitor::statusReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
        OpcMonitor* monitor=static_cast<OpcMonitor*>(nodeContext);
        monitor->updateStatusVariable(server);
        }



void OpcMonitor::addValueCallback(UA_Server *server,UA_NodeId variableId,
                                   void (*readCallback)(UA_Server *server, const UA_NodeId *sessionId,
                                                         void *sessionContext, const UA_NodeId *nodeid,
                                                         void *nodeContext, const UA_NumericRange *range,
                                                         const UA_DataValue *value)) {
        UA_ValueCallback callback;
        callback.onRead = readCallback;
        callback.onWrite= nullptr;
        UA_Server_setVariableNode_valueCallback(server, variableId, callback);
        }

UA_StatusCode OpcMonitor::disconnectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    OpcMonitor* monitor=static_cast<OpcMonitor*>(methodContext);
    monitor->disconnectDevice();
    return UA_STATUSCODE_GOOD;
}

void OpcMonitor::addDisconnectDeviceMethod(UA_Server *server) {
    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Disconnect device");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","disconnect");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName=UA_QUALIFIEDNAME_ALLOC(1,"disconnect");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &disconnectDeviceCallback,
                            0, nullptr, 0, nullptr, this, nullptr);
    UA_QualifiedName_deleteMembers(&methodQName);
    UA_MethodAttributes_deleteMembers(&methodAttr);
}

UA_StatusCode OpcMonitor::connectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {

    OpcMonitor* monitor=static_cast<OpcMonitor*>(methodContext);
    UA_Int32 port = *(UA_Int32*)input[1].data;
    UA_String host = *(UA_String*)input[0].data;
    std::string address(reinterpret_cast<char*>(host.data));
    TCPConnectionParameters parameters;
    parameters.IPaddress=address;
    parameters.port=port;
    monitor->connectDevice(&parameters);
    return UA_STATUSCODE_GOOD;
}
void OpcMonitor::addConnectDeviceMethod(UA_Server *server) {

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

    UA_QualifiedName methodQName=UA_QUALIFIEDNAME_ALLOC(1,"connect");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &connectDeviceCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_QualifiedName_deleteMembers(&methodQName);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
}


/*
void OpcMonitor::addMonitoredItem(UA_Server *server,UA_NodeId VariableId,UA_Double sampling) {
  //  UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, VariableName.c_str());
    UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(VariableId);
    monRequest.requestedParameters.samplingInterval = sampling; // 100 ms interval
    UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, nullptr, dataChangeNotificationCallback);
  //  UA_NodeId_deleteMembers(&NodeId);
}


void OpcMonitor::dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                              void *monitoredItemContext, const UA_NodeId *nodeId,
                              void *nodeContext, UA_UInt32 attributeId,
                              const UA_DataValue *value) {
   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Notification");
}
*/
void OpcMonitor::spawn_thread(){
    thread_running=true;
    device_thread=std::thread([this]{run_thread();});
}
void OpcMonitor::join_thread(){
    thread_running=false;
    device_thread.join();
}
