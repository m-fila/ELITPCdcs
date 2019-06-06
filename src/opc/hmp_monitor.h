#ifndef HMP_MONITOR_H
#define HMP_MONITOR_H


#include <open62541/server.h>
#include <iostream>
#include <open62541/client_subscriptions.h>
#include <map>
#include <string>
template <class T>
class hmp_monitor: public opc_abstract_monitor<HMPMeasurements>
{
public:
    opc_abstract_monitor(){}
    ~opc_abstract_monitor(){}
    char* VariableName;
    UA_DataType VariableType;
    virtual T getMeasurements()=0;
    virtual void addVariable(UA_Server *server)=0;
    void addVariable(UA_Server *server,T variable){
        UA_VariableAttributes vattr = UA_VariableAttributes_default;
        char* language=const_cast<char*>("en-Us");
        vattr.description = UA_LOCALIZEDTEXT(language, VariableName);
        vattr.displayName = UA_LOCALIZEDTEXT(language, VariableName);
        vattr.dataType = VariableType.typeId;
        vattr.valueRank = UA_VALUERANK_SCALAR;
        UA_Variant_setScalar(&vattr.value, &variable, &VariableType);

        UA_Server_addVariableNode(server, UA_NODEID_STRING(1, VariableName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                  UA_QUALIFIEDNAME(1, VariableName),
                                  VariableType.typeId, vattr, nullptr, nullptr);
    }

    void updateVariable(UA_Server *server){
        T now =this->getMeasurements();
        UA_Variant value;
        UA_Variant_setScalar(&value, &now, &VariableType);
        UA_NodeId NodeId = UA_NODEID_STRING(1, VariableName);
        UA_Server_writeValue(server, NodeId, value);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Variable updated");
    }


     static void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                                   void *monitoredItemContext, const UA_NodeId *nodeId,
                                   void *nodeContext, UA_UInt32 attributeId,
                                   const UA_DataValue *value) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Notification");
    }

    void addMonitoredItem(UA_Server *server) {
        UA_NodeId NodeId = UA_NODEID_STRING(1, VariableName);
        //UA_NodeId NodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(NodeId);
        monRequest.requestedParameters.samplingInterval = 500.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                                monRequest, nullptr, dataChangeNotificationCallback);
    }


   virtual void beforeReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){std::cout<<"oooooooooo"<<std::endl;};
   /* static void afterWriteCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The variable was updated");
    }*/


    void addValueCallback(UA_Server *server) {
        UA_NodeId currentNodeId = UA_NODEID_STRING(1, VariableName);
        UA_ValueCallback callback ;

        //u_ptm_cast a;
        //a.pmember=&opc_abstract_monitor<T>::beforeReadCallback;
        callback.onRead = beforeReadCallback;
      callback.onWrite=NULL;
        UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);

    }


};
#endif // HMP_MONITOR

