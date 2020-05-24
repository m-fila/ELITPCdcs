#include "opc_monitor.h"

OpcMonitor::OpcMonitor(std::string name,UA_DataType typeM, UA_DataType typeC): 
    OpcObject(name),
    thread_running(false),
    variableTypeM(typeM),
    variableTypeC(typeC),
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

UA_StatusCode OpcMonitor::connectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {

    OpcMonitor* monitor=static_cast<OpcMonitor*>(methodContext);
    UA_Int32 port = *(UA_Int32*)input[1].data;
    UA_String host = *(UA_String*)input[0].data;
    if( host.length==0){
    return UA_STATUSCODE_GOOD;
    }
    std::string address(reinterpret_cast<char*>(host.data));
    TCPConnectionParameters parameters;
    parameters.IPaddress=address;
    parameters.port=port;
    monitor->connectDevice(&parameters);
    return UA_STATUSCODE_GOOD;
}

void OpcMonitor::addConnectDeviceMethod(UA_Server *server){
    addMethod(server,"connect",connectDeviceCallback,"Connect device",{{UA_TYPES[UA_TYPES_STRING],"Address"," Host's address"},{UA_TYPES[UA_TYPES_INT32],"Port"," Host's port"}},{});
}
void OpcMonitor::addDisconnectDeviceMethod(UA_Server *server){
    addMethod(server,"disconnect",disconnectDeviceCallback,"Disconnect device",{},{});
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
