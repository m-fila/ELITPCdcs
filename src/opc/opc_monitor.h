#ifndef OPC_MONITOR_H
#define OPC_MONITOR_H
#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/client_subscriptions.h>
#include "../ConnectionParameters.h"
class opc_monitor
{
public:
    opc_monitor(){}
    opc_monitor(std::string name);
    ~opc_monitor(){}


    const std::string ObjectName;
    UA_NodeId ObjectNodeId;
    const std::string MeasurementsVariableName;
    const std::string SettingsVariableName;
    const std::string StatusVariableName;

    UA_DataType VariableType;

    void addObject(UA_Server *server);
    void addMeasurementsVariable(UA_Server *server){addCustomTypeVariable(server,MeasurementsVariableName);}
    void addSettingsVariable(UA_Server *server){addCustomTypeVariable(server,SettingsVariableName);}
    void addCustomTypeVariable(UA_Server *server,std::string VariableName);
    void addStatusVariable(UA_Server *server);

    virtual void disconnectDevice()=0;
    virtual void connectDevice(TCPConnectionParameters* parameters)=0;
        //updating via value callback:
    void addValueCallback(UA_Server *server,std::string VariableName,  void (*ReadCallback)(UA_Server *server, const UA_NodeId *sessionId,
                                                                                      void *sessionContext, const UA_NodeId *nodeid,
                                                                                      void *nodeContext, const UA_NumericRange *range,
                                                                   const UA_DataValue *value));
    static void MeasurementsReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void SettingsReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void StatusReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);

    virtual void updateMeasurements(UA_Server *server)=0;
    virtual void updateSettings(UA_Server *server)=0;
    virtual void updateStatus(UA_Server *server)=0;
     //generic device methods:
     void addDisconnectDeviceMethod(UA_Server *server);
     static UA_StatusCode DisconnectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);

     void addConnectDeviceMethod(UA_Server *server);
     static UA_StatusCode ConnectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);


        // self monitoring via MonitoredItem:
        //     (alternative to monitoring by TimedCallback defined in opc_server)
     void addMonitoredItem(UA_Server *server,std::string VariableName, UA_Double sampling=500.0);
     static void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                                   void *monitoredItemContext, const UA_NodeId *nodeId,
                                   void *nodeContext, UA_UInt32 attributeId,
                                   const UA_DataValue *value);


};

#endif // OPC_MONITOR_H
