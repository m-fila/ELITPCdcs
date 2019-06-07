#include "opc_monitor.h"
#include <iostream>
void opc_monitor::beforeReadCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeid, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data){
       // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Onward!");
        std::string NodeName=reinterpret_cast<char*>(nodeid->identifier.string.data);
      // opc_monitor* Monitor=NodeMap::Nodes[NodeName];
       opc_monitor* Monitor=static_cast<opc_monitor*>(nodeContext);
    //  std::cout<<"!!!!"<<Monitor->MeasurementsVariableName <<"!!!!!"<<std::endl;
        Monitor->updateVariable(server);

        //std::cout<<NodeMap::insta->VariableName<<std::endl;
        //this->updateVariable(server);
         //   opc_callbacks* insta=NodeMap::data[const_cast<char*>("HMPVariable")];
         // std::cout<< insta<<std::endl;
        }



void opc_monitor::afterWriteCallback(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The variable was updated");
        }

void opc_monitor::addValueCallback(UA_Server *server) {
        UA_NodeId currentNodeId = UA_NODEID_STRING(1, MeasurementsVariableName);
        UA_ValueCallback callback ;
        callback.onRead = beforeReadCallback;
        callback.onWrite= afterWriteCallback;
        UA_Server_setVariableNode_valueCallback(server, currentNodeId, callback);
        }

UA_StatusCode opc_monitor::DisconnectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Disconnect was called");
    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "%d",objectId);
    //std::string NodeName=reinterpret_cast<char*>(objectId->identifier.string.data);
    //opc_monitor* Monitor=NodeMap::Nodes[NodeName];
    opc_monitor* Monitor=static_cast<opc_monitor*>(methodContext);
    Monitor->disconnectDevice();
    return UA_STATUSCODE_GOOD;
}

void opc_monitor::addDisconnectDeviceMethod(UA_Server *server) {
    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT("en-US","Disconnect device");
    helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","disconnect");
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(1,"DisconnectDevice"),
                            UA_NODEID_STRING(1,ObjectName),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "disconnect"),
                            helloAttr, &DisconnectDeviceCallback,
                            0, nullptr, 0, nullptr, this, nullptr);
}

UA_StatusCode opc_monitor::ConnectDeviceCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Connect was called");

    opc_monitor* Monitor=static_cast<opc_monitor*>(methodContext);
    // std::string NodeName=reinterpret_cast<char*>(objectId->identifier.string.data);
    // opc_monitor* Monitor=NodeMap::Nodes[NodeName];
    UA_Int32 port = *(UA_Int32*)input[1].data;
    UA_String host = *(UA_String*)input[0].data;
    std::string address(reinterpret_cast<char*>(host.data));


    TCPConnectionParameters* parameters=new TCPConnectionParameters;
    parameters->IPaddress=address;
    parameters->port=port;
    Monitor->connectDevice(parameters);
    return UA_STATUSCODE_GOOD;
}
void opc_monitor::addConnectDeviceMethod(UA_Server *server) {

    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT("en-US", "Host");
    inputArguments[0].name = UA_STRING("Host address");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT("en-US", "Port");
    inputArguments[1].name = UA_STRING("TCP port");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT("en-US","Connect device");
    helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","connect");
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(1,"ConnectDevice"),
                            UA_NODEID_STRING(1,ObjectName),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Connect"),
                            helloAttr, &ConnectDeviceCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
}
