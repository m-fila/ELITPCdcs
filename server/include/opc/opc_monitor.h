#ifndef OPC_MONITOR_H
#define OPC_MONITOR_H
#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/client_subscriptions.h>
#include "../ConnectionParameters.h"
#include "opc_object.h"
#include <iostream>
#include <mutex>
#include <thread>


class OpcMonitor: public OpcObject
{
public:
    OpcMonitor(std::string name);
    virtual ~OpcMonitor();
    virtual void init(UA_Server *server)=0;
    void addMeasurementsVariable(UA_Server *server);
    void addConfigurationVariable(UA_Server *server);
    void addStatusVariable(UA_Server *server);
    void addDisconnectDeviceMethod(UA_Server *server);
    void addConnectDeviceMethod(UA_Server *server);
    void addValueCallback(UA_Server *server,UA_NodeId VariableId,
                          void (*ReadCallback)(UA_Server *server, const UA_NodeId *sessionId,
                                                void *sessionContext, const UA_NodeId *nodeid,
                                                void *nodeContext, const UA_NumericRange *range,
                                                const UA_DataValue *value));
//    void addMonitoredItem(UA_Server *server,UA_NodeId VariableId, UA_Double sampling=500.0);


protected:
    void spawn_thread();
    void join_thread();
    virtual void run_thread()=0;

    virtual void updateMeasurementsVariable(UA_Server *server)=0;
    virtual void updateConfigurationVariable(UA_Server *server)=0;
    virtual void updateStatusVariable(UA_Server *server)=0;

    virtual void disconnectDevice()=0;
    virtual void connectDevice(TCPConnectionParameters* parameters)=0;

    static void measurementsReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void configurationReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void statusReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);

    bool thread_running;

    UA_NodeId measurementsId;
    UA_NodeId configurationId;
    UA_NodeId statusId;
    UA_DataType variableTypeM;
    UA_DataType variableTypeC;

private:
 /*   static void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                                   void *monitoredItemContext, const UA_NodeId *nodeId,
                                   void *nodeContext, UA_UInt32 attributeId,
                                   const UA_DataValue *value);
*/
    static UA_StatusCode disconnectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);

    static UA_StatusCode connectDeviceCallback(UA_Server *server,
                              const UA_NodeId *sessionId, void *sessionHandle,
                              const UA_NodeId *methodId, void *methodContext,
                              const UA_NodeId *objectId, void *objectContext,
                              size_t inputSize, const UA_Variant *input,
                              size_t outputSize, UA_Variant *output);

     const std::string measurementsVariableName;
     const std::string configurationVariableName;
     const std::string statusVariableName;
     std::thread device_thread;

};

#endif // OPC_MONITOR_H
