#include "DCSVariable.h"

DCSVariable::DCSVariable(UA_Server *server, UA_NodeId parentNodeId, std::string name, UA_DataType type)
    : server(server), variableName(name), dataType(type){
            UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", name.c_str());
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", name.c_str());
    attr.dataType = dataType.typeId;
    attr.valueRank = UA_VALUERANK_SCALAR;
    UA_QualifiedName qName=UA_QUALIFIEDNAME_ALLOC(1,variableName.c_str());
    UA_Server_addVariableNode(server, UA_NODEID_NULL,
                              parentNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              qName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, this, &variableNodeId);
    UA_VariableAttributes_deleteMembers(&attr);
    UA_QualifiedName_deleteMembers(&qName);
    }