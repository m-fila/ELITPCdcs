#include "include/state.h"
#include <iostream>
stateMachine::stateMachine( std::string OName,QObject *parent): opcQObject(OName,parent){
     VariableName=ObjectName+".State";
//    VariableNodeId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
    MethodNodeId =UA_NODEID_STRING(1,const_cast<char*>("SetState"));
}

stateMachine::~stateMachine(){
  //  UA_NodeId_deleteMembers(&VariableNodeId);
}

void stateMachine::changeState(int index){
//std::cout<<index<<std::endl;

    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &index, &UA_TYPES[UA_TYPES_INT32]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                MethodNodeId, 1, &input, nullptr,nullptr);
//   std::cout<<UA_StatusCode_name(retval)<<std::endl;
    UA_Variant_clear(&input);
}

void stateMachine::StateChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value){
    int index=*static_cast<int*>(value->value.data);
    stateMachine* context=static_cast<stateMachine*>(monContext);
    emit context->stateChanged(index);

}

void stateMachine::opcInit(UA_Client *Client, UA_ClientConfig *Config, UA_CreateSubscriptionResponse response){
    client=Client;
    config=Config;
    addMonitoredItem(VariableName,response,StateChangedCallback);

}
