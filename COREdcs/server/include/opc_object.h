#ifndef OPC_OBJECT_H
#define OPC_OBJECT_H
#include <open62541/server.h>
#include <open62541/plugin/log_stdout.h>
#include <string>
class OpcObject
{
public:
    OpcObject(std::string name);
    ~OpcObject();
    void addObject(UA_Server *server);
protected:
    virtual void addVariable3(UA_Server *server,UA_NodeId *VariableNodeId,std::string VariableName,UA_DataType VariableType);
    virtual UA_NodeId addVariable2(UA_Server *server,std::string VariableName,UA_DataType VariableType,UA_NodeId TypeNodeId); //auto-asign nodeid and return it
    virtual void addVariable(UA_Server *server,std::string VariableName,UA_DataType VariableType,UA_NodeId TypeNodeId); //nodeid_string(1,VariableName)
    const std::string objectName;
    UA_NodeId objectNodeId;
};

#endif // OPC_OBJECT_H
