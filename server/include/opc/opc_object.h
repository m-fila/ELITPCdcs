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

    const std::string ObjectName;
    UA_NodeId ObjectNodeId;
    void addObject(UA_Server *server);
    virtual void addVariable(UA_Server *server,std::string VariableName,UA_DataType VariableType,UA_NodeId TypeNodeId);
};

#endif // OPC_OBJECT_H
