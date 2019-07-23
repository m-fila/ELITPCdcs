#include "../include/abstract_variable.h"
#include "../../common/HMPDataType.h"
#include <iostream>
abstract_variable::abstract_variable(std::string ObjectName,std::string VariableName) : ObjectName(ObjectName), VariableName(VariableName), FullName(ObjectName+"."+VariableName)
{

   // VariableNodeId=UA_NODEID_STRING_ALLOC(1,FullName.c_str());
}
abstract_variable::~abstract_variable(){
  //  UA_NodeId_deleteMembers(&VariableNodeId);
}

void abstract_variable::handler_ValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                                             UA_UInt32 monId, void *monContext, UA_DataValue *value){
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"notification!");
     abstract_variable *variable=static_cast<abstract_variable*>(monContext);
     variable->updateData(value);
     database* Database=static_cast<database*>(subContext);
     Database->logData(variable->FullName, std::to_string(value->sourceTimestamp)+","+variable->translateValue());
}

void abstract_variable::addMonitoredItem(UA_Client *client,UA_CreateSubscriptionResponse response){
     UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(VariableNodeId);
     std::cout<<VariableNodeId.identifier.numeric<<std::endl;
     UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
                                                                                         UA_TIMESTAMPSTORETURN_BOTH,
                                                                                         monRequest, this, handler_ValueChanged, nullptr);
     if(monResponse.statusCode == UA_STATUSCODE_GOOD){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"created monitored item");
     }
}

void abstract_variable::init(UA_Client *client){
    UA_NodeId ObjectNodeId=UA_NODEID_STRING_ALLOC(1,ObjectName.c_str());
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
                if(!str.compare(VariableName)){
                    VariableNodeId=ref.nodeId.nodeId;
                    std::cout<<str<<std::endl;
                    std::cout<<VariableNodeId.identifier.numeric<<std::endl;}
            }
    }
    UA_NodeId_deleteMembers(&ObjectNodeId);
}
