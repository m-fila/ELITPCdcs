#ifndef OPC_OBJECT_H
#define OPC_OBJECT_H
#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <string>
class opc_object
{
public:
    opc_object(std::string name);
    ~opc_object();
    void addObject(UA_Server *server);
protected:
    virtual UA_NodeId addVariable2(UA_Server *server,std::string VariableName,UA_DataType VariableType,UA_NodeId TypeNodeId); //auto-asign nodeid and return it
    virtual void addVariable(UA_Server *server,std::string VariableName,UA_DataType VariableType,UA_NodeId TypeNodeId); //nodeid_string(1,VariableName)
    const std::string ObjectName;
    UA_NodeId ObjectNodeId;
};

#endif // OPC_OBJECT_H
