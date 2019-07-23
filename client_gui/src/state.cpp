#include "include/state.h"
#include <iostream>
stateMachine::stateMachine( std::string OName,QObject *parent): opcQObject(OName,parent){
     VariableName="State";
//    VariableNodeId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
//    MethodNodeId =UA_NODEID_STRING(1,const_cast<char*>("SetState"));
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
    browseIds();
    addMonitoredItem(VariableNodeId,response,StateChangedCallback);

}

void stateMachine::browseIds(){
    std::string MethodBrowseName="setstate";

    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    bReq.nodesToBrowse[0].nodeId =ObjectNodeId;// UA_NODEID_NUMERIC(0, UA_NS0ID_OBJE
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_BROWSENAME;
    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    for(size_t i = 0; i < bResp.resultsSize; ++i) {
            for(size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription ref = (bResp.results[i].references[j]);
                std::string str =std::string(reinterpret_cast<char*>(ref.browseName.name.data));
                if(!str.compare(VariableName))
                    VariableNodeId=ref.nodeId.nodeId;
                if(!str.compare(MethodBrowseName))
                    MethodNodeId=ref.nodeId.nodeId;
            }
    }

}
