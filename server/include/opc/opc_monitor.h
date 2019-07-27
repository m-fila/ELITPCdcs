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
#include <chrono>

class opc_monitor: public opc_object
{
public:

    void spawn_thread(){updateThread=std::thread([this]{update();});}
    void join_thread(){updateThread.join();}

    opc_monitor(std::string name);
    virtual ~opc_monitor();
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
    void addMonitoredItem(UA_Server *server,UA_NodeId VariableId, UA_Double sampling=500.0);

protected:
    void update(){
        for(int i=0; i<10000;i++){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //prints();
            updateConfiguration();
            updateMeasurements();
        }
    }
    void prints(){
      std::lock_guard<std::mutex> lock(deviceMutex);
      std::cout<<ObjectName<<std::endl;
    }
    std::mutex deviceMutex;
    std::mutex mMutex;
    std::mutex cMutex;
    std::thread updateThread;

    UA_NodeId MeasurementsId;
    UA_NodeId ConfigurationId;
    UA_NodeId StatusId;
    UA_DataType VariableTypeM;
    UA_DataType VariableTypeC;

    virtual void updateMeasurementsVariable(UA_Server *server)=0;
    virtual void updateConfigurationVariable(UA_Server *server)=0;
    virtual void updateMeasurements()=0;
    virtual void updateConfiguration()=0;
    virtual void updateStatusVariable(UA_Server *server)=0;
    virtual void disconnectDevice()=0;
    virtual void connectDevice(TCPConnectionParameters* parameters)=0;

    static void MeasurementsReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void ConfigurationReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);
    static void StatusReadCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeid, void *nodeContext,
                       const UA_NumericRange *range, const UA_DataValue *data);

private:
    static void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                                   void *monitoredItemContext, const UA_NodeId *nodeId,
                                   void *nodeContext, UA_UInt32 attributeId,
                                   const UA_DataValue *value);

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
     const std::string MeasurementsVariableName;
     const std::string ConfigurationVariableName;
     const std::string StatusVariableName;

};

#endif // OPC_MONITOR_H
