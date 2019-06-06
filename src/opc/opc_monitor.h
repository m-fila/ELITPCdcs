#ifndef OPC_MONITOR_H
#define OPC_MONITOR_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <map>
#include "../ConnectionParameters.h"
class opc_monitor
{
public:
    opc_monitor(){}
    ~opc_monitor(){}


    char*  ObjectName;
    char* MeasurementsVariableName;
    char* StatusVariableName;

    UA_DataType VariableType;
    virtual void updateVariable(UA_Server *server)=0;
    virtual void disconnectDevice()=0;
    virtual void connectDevice(TCPConnectionParameters* parameters)=0;

    static void beforeReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);


     static void afterWriteCallback(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    const UA_NumericRange *range, const UA_DataValue *data);


     void addValueCallback(UA_Server *server);

     void addDisconnectDeviceMethod(UA_Server *server);

     static UA_StatusCode DisconnectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);


     static UA_StatusCode ConnectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);
     void addConnectDeviceMethod(UA_Server *server);

/*
     static UA_StatusCode SettingsMethodCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);
*/
};

#endif // OPC_MONITOR_H
