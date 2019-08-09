#include "../../include/opc/opc_object.h"

OpcObject::OpcObject(std::string name): objectName(name){
    objectNodeId=UA_NODEID_STRING_ALLOC(1,name.c_str());
}
OpcObject::~OpcObject(){
    UA_NodeId_deleteMembers(&objectNodeId);
}

void OpcObject::addObject(UA_Server *server){
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_QualifiedName NodeQName=UA_QUALIFIEDNAME_ALLOC(1,objectName.c_str());
    oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", objectName.c_str());
    UA_Server_addObjectNode(server, objectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              NodeQName, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                              oAttr, this, nullptr);
    UA_ObjectAttributes_deleteMembers(&oAttr);
    UA_QualifiedName_deleteMembers(&NodeQName);
}

void OpcObject::addVariable(UA_Server *server,std::string VariableName,UA_DataType VariableType, UA_NodeId TypeNodeId){
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    vattr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", VariableName.c_str());
    vattr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", VariableName.c_str());
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId VariableNodeId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    UA_QualifiedName VariableQName=UA_QUALIFIEDNAME_ALLOC(1,VariableName.c_str());
    UA_Server_addVariableNode(server, VariableNodeId,
                              objectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              VariableQName,
                              TypeNodeId, vattr, this, nullptr);
    UA_VariableAttributes_deleteMembers(&vattr);
    UA_NodeId_deleteMembers(&VariableNodeId);
    UA_QualifiedName_deleteMembers(&VariableQName);
}

UA_NodeId OpcObject::addVariable2(UA_Server *server,std::string VariableName,UA_DataType VariableType, UA_NodeId TypeNodeId){
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    vattr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", VariableName.c_str());
    vattr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", VariableName.c_str());
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId VariableNodeId;//=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    UA_QualifiedName VariableQName=UA_QUALIFIEDNAME_ALLOC(1,VariableName.c_str());
    UA_Server_addVariableNode(server, UA_NODEID_NULL,
                              objectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              VariableQName,
                              TypeNodeId, vattr, this, &VariableNodeId);
    UA_VariableAttributes_deleteMembers(&vattr);
//    UA_NodeId_deleteMembers(&VariableNodeId);
    UA_QualifiedName_deleteMembers(&VariableQName);
    return VariableNodeId;
}

void OpcObject::addVariable3(UA_Server *server,UA_NodeId *VariableNodeId,std::string VariableName,UA_DataType VariableType){
    UA_VariableAttributes vattr = UA_VariableAttributes_default;
    vattr.description = UA_LOCALIZEDTEXT_ALLOC ("en-Us", VariableName.c_str());
    vattr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", VariableName.c_str());
    vattr.dataType = VariableType.typeId;
    vattr.valueRank = UA_VALUERANK_SCALAR;
    UA_QualifiedName VariableQName=UA_QUALIFIEDNAME_ALLOC(1,VariableName.c_str());
    UA_Server_addVariableNode(server, UA_NODEID_NULL,
                              objectNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              VariableQName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vattr, this, VariableNodeId);
    UA_VariableAttributes_deleteMembers(&vattr);
    UA_QualifiedName_deleteMembers(&VariableQName);
}
