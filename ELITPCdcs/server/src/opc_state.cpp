#include "opc_state.h"
#include <iostream>
OpcState::OpcState(): OpcObject("MachineState"),state(MachineState::Idle){
    VariableName="State";
}


OpcState::~OpcState(){
    UA_NodeId_deleteMembers(&objectNodeId);
}

void OpcState::init(UA_Server *server){
    addObject(server);
    addVariable3(server,&VariableNodeId,VariableName,UA_TYPES[UA_TYPES_INT32]);
    setState(server,state);
    addSetStateMethod(server);
}

void OpcState::setState(UA_Server *server, MachineState State){
    UA_Variant value;
    UA_Variant_setScalar(&value, &State, &UA_TYPES[UA_TYPES_INT32]);
 //   UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, VariableName.c_str());
    UA_Server_writeValue(server, VariableNodeId, value);
 //   UA_NodeId_deleteMembers(&NodeId);
}


UA_StatusCode OpcState::setStateCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetState was called");
    OpcState* AssociatedObj=static_cast<OpcState*>(methodContext);

    MachineState state = *(MachineState*)input->data;
    AssociatedObj->setState(server,state);
    AssociatedObj->state=state;


    return UA_STATUSCODE_GOOD;
}
void OpcState::addSetStateMethod(UA_Server *server) {

    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "state");
    inputArgument.name = UA_String_fromChars("State");
    inputArgument.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Changes state of machine");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setstate");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetState");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setstate");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &setStateCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArgument);
 //   UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}


