#include "../../include/opc/opc_object.h"

opc_object::opc_object(std::string name): ObjectName(name){
    ObjectNodeId=UA_NODEID_STRING_ALLOC(1,name.c_str());
}
opc_object::~opc_object(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
}

void opc_object::addObject(UA_Server *server){
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_QualifiedName NodeQName=UA_QUALIFIEDNAME_ALLOC(1,ObjectName.c_str());
    oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", ObjectName.c_str());
    UA_Server_addObjectNode(server, ObjectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              NodeQName, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                              oAttr, this, nullptr);
    UA_ObjectAttributes_deleteMembers(&oAttr);
    UA_QualifiedName_deleteMembers(&NodeQName);
}

void opc_object::addVariable(UA_Server *server,std::string VariableName,UA_DataType VariableType, UA_NodeId TypeNodeId){
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    vattr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", VariableName.c_str());
    vattr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", VariableName.c_str());
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId VariableNodeId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    UA_QualifiedName VariableQName=UA_QUALIFIEDNAME_ALLOC(1,VariableName.c_str());
    UA_Server_addVariableNode(server, VariableNodeId,
                              ObjectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              VariableQName,
                              TypeNodeId, vattr, this, nullptr);
    UA_VariableAttributes_deleteMembers(&vattr);
    UA_NodeId_deleteMembers(&VariableNodeId);
    UA_QualifiedName_deleteMembers(&VariableQName);
}

UA_NodeId opc_object::addVariable2(UA_Server *server,std::string VariableName,UA_DataType VariableType, UA_NodeId TypeNodeId){
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    vattr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", VariableName.c_str());
    vattr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", VariableName.c_str());
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId VariableNodeId;//=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    UA_QualifiedName VariableQName=UA_QUALIFIEDNAME_ALLOC(1,VariableName.c_str());
    UA_Server_addVariableNode(server, UA_NODEID_NULL,
                              ObjectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              VariableQName,
                              TypeNodeId, vattr, this, &VariableNodeId);
    UA_VariableAttributes_deleteMembers(&vattr);
//    UA_NodeId_deleteMembers(&VariableNodeId);
    UA_QualifiedName_deleteMembers(&VariableQName);
    return VariableNodeId;
}
