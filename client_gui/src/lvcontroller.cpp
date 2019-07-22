#include "include/lvcontroller.h"
#include <iostream>
lv_controller::lv_controller(std::string OName, QObject *parent):
    opc_controller(OName, parent)
{
}
void lv_controller::callSetOutput(bool state){
       UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input;
       UA_Variant_init(&input);
       UA_Variant_setScalarCopy(&input, &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   setOutputNodeId, 1, &input, nullptr,nullptr);
       UA_Variant_clear(&input);
}

void lv_controller::callSetChannel(int nr, bool state){
    UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                setChannelNodeId, 2, input, nullptr,nullptr);
    UA_Variant_clear(input);
}

void lv_controller::browseIds(){
    std::string connectBrowseName="connect";
    std::string disconnectBrowseName="disconnect";
    std::string setChannelBrowseName="setchannel";
    std::string setOutputBrowseName="setoutput";
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
                if(!str.compare(MeasurementsVariableName))
                    MeasurementsNodeId=ref.nodeId.nodeId;
                if(!str.compare(ConfigurationVariableName))
                    ConfigurationNodeId=ref.nodeId.nodeId;
                if(!str.compare(StatusVariableName))
                    StatusNodeId=ref.nodeId.nodeId;
                if(!str.compare(setChannelBrowseName))
                    setChannelNodeId=ref.nodeId.nodeId;
                if(!str.compare(setOutputBrowseName))
                    setOutputNodeId=ref.nodeId.nodeId;
                if(!str.compare(connectBrowseName))
                    connectNodeId=ref.nodeId.nodeId;
                if(!str.compare(disconnectBrowseName))
                    disconnectNodeId=ref.nodeId.nodeId;

            }
    }
}
