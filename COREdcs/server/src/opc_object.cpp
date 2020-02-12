#include "opc_object.h"

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

void OpcObject::addMethod(UA_Server *server, std::string methodName, UA_MethodCallback methodCallback,
                          std::string methodDescription,std::vector<methodInputOutput> inputs,
                          std::vector<methodInputOutput> outputs,UA_NodeId *methodNodeId){
    UA_Argument *inputArguments=static_cast<UA_Argument*>(UA_Array_new(inputs.size(),&UA_TYPES[UA_TYPES_ARGUMENT]));
    for (int i=0; i<inputs.size();i++){
        UA_Argument_init(&inputArguments[i]);
        inputArguments[i].description = UA_LOCALIZEDTEXT_ALLOC("en-US", inputs.at(i).description.c_str());
        inputArguments[i].name = UA_String_fromChars(inputs.at(i).name.c_str());
        inputArguments[i].dataType = inputs.at(i).dataType.typeId;
        inputArguments[i].valueRank = UA_VALUERANK_SCALAR;
    }
    UA_Argument *outputArguments=static_cast<UA_Argument*>(UA_Array_new(outputs.size(),&UA_TYPES[UA_TYPES_ARGUMENT]));
    for (int i=0; i<outputs.size();++i){
        UA_Argument_init(&outputArguments[i]);
        outputArguments[i].description = UA_LOCALIZEDTEXT_ALLOC("en-US", outputs.at(i).description.c_str());
        outputArguments[i].name = UA_String_fromChars(outputs.at(i).name.c_str());
        outputArguments[i].dataType = outputs.at(i).dataType.typeId;
        outputArguments[i].valueRank = UA_VALUERANK_SCALAR;
    }
    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US",methodDescription.c_str());
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US",methodName.c_str());
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName= UA_QUALIFIEDNAME_ALLOC(1, methodName.c_str());
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, methodCallback,
                            inputs.size(),inputArguments, outputs.size(), outputArguments,this, methodNodeId);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_QualifiedName_deleteMembers(&methodQName);

    
    //for(int i=0;i<inputs.size();++i){
    //    UA_Argument_deleteMembers(&inputArguments[i]);
    //}
    //UA_Argument_delete(inputArguments);
    //for(int i=0;i<outputs.size();++i){
    //    UA_Argument_deleteMembers(&outputArguments[i]);
    //}
    //UA_Argument_delete(outputArguments);
    
    UA_Array_delete(inputArguments,inputs.size(),&UA_TYPES[UA_TYPES_ARGUMENT]);
    UA_Array_delete(outputArguments,outputs.size(),&UA_TYPES[UA_TYPES_ARGUMENT]);
}

    
