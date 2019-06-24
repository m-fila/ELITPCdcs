#include "opc_state.h"
#include <iostream>
opc_state::opc_state(): opc_object("MachineState"),state(0){
    VariableName=ObjectName+".State";
}


opc_state::~opc_state(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
}

void opc_state::init(UA_Server *server){
    addObject(server);
    addVariable(server,VariableName.c_str(),UA_TYPES[UA_TYPES_INT32],UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));
    setState(server,MachineState::Idle);
    addSetStateMethod(server);
}

void opc_state::setState(UA_Server *server, MachineState state){
    UA_Variant value;
    UA_Variant_setScalar(&value, &state, &UA_TYPES[UA_TYPES_INT32]);
    UA_NodeId NodeId = UA_NODEID_STRING_ALLOC(1, VariableName.c_str());
    UA_Server_writeValue(server, NodeId, value);
    UA_NodeId_deleteMembers(&NodeId);
}


UA_StatusCode opc_state::SetStateCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    opc_state* AssociatedObj=static_cast<opc_state*>(methodContext);

    MachineState state = *(MachineState*)input->data;
    AssociatedObj->setState(server,state);
    AssociatedObj->state=state;


    return UA_STATUSCODE_GOOD;
}
void opc_state::addSetStateMethod(UA_Server *server) {

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
    UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetState");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setstate");
    UA_Server_addMethodNode(server, MethodNodeId,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetStateCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArgument);
    UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}


