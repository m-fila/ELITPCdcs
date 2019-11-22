#include "include/hvcontroller.h"
#include <iostream>
hv_controller::hv_controller(std::string OName, QObject *parent):
    opc_controller(OName, parent),
    setChannelBrowseName("setchannel"),
    setVoltageBrowseName("setvoltage"),
    setVoltageMaxBrowseName("setvoltagemax"),
    setRampUpBrowseName("setrampup"),
    setRampDownBrowseName("setrampdown"),
    setCurrentBrowseName("setcurrent")
{
}



void hv_controller::callSetChannel(int nr, bool state){
  //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                browsedIds[setChannelBrowseName], 2, input, nullptr,nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetVoltage(int nr,double voltage){
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input[2];
       UA_Variant_init(input);
       UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
       UA_Variant_setScalarCopy(&input[1], &voltage, &UA_TYPES[UA_TYPES_DOUBLE]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   browsedIds[setVoltageBrowseName], 2, input, nullptr,nullptr);
       UA_Variant_clear(input);
}

void hv_controller::callSetVoltageMax(int nr,double voltage){
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input[2];
       UA_Variant_init(input);
       UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
       UA_Variant_setScalarCopy(&input[1], &voltage, &UA_TYPES[UA_TYPES_DOUBLE]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   browsedIds[setVoltageMaxBrowseName], 2, input, nullptr,nullptr);
       UA_Variant_clear(input);
}

void hv_controller::callSetRampUp(int nr,double rup){
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input[2];
       UA_Variant_init(input);
       UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
       UA_Variant_setScalarCopy(&input[1], &rup, &UA_TYPES[UA_TYPES_DOUBLE]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   browsedIds[setRampUpBrowseName], 2, input, nullptr,nullptr);
       UA_Variant_clear(input);
}

void hv_controller::callSetRampDown(int nr,double rdwn){
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input[2];
       UA_Variant_init(input);
       UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
       UA_Variant_setScalarCopy(&input[1], &rdwn, &UA_TYPES[UA_TYPES_DOUBLE]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   browsedIds[setRampDownBrowseName], 2, input, nullptr,nullptr);
       UA_Variant_clear(input);
}

void hv_controller::callSetCurrent(int nr,double current){
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
       UA_Variant input[2];
       UA_Variant_init(input);
       UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
       UA_Variant_setScalarCopy(&input[1], &current, &UA_TYPES[UA_TYPES_DOUBLE]);
       UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                   browsedIds[setCurrentBrowseName], 2, input, nullptr,nullptr);
       UA_Variant_clear(input);
}
/*void hv_controller::browseIds(){
    std::string connectBrowseName="connect";
    std::string disconnectBrowseName="disconnect";
    std::string setChannelBrowseName="setchannel";
    std::string setVoltageBrowseName="setvoltage";
    std::string setVoltageMaxBrowseName="setvoltagemax";
    std::string setRampUpBrowseName="setrampup";
    std::string setRampDownBrowseName="setrampdown";
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
                if(!str.compare(connectBrowseName))
                    connectNodeId=ref.nodeId.nodeId;
                if(!str.compare(disconnectBrowseName))
                    disconnectNodeId=ref.nodeId.nodeId;
                if(!str.compare(setChannelBrowseName))
                    setChannelNodeId=ref.nodeId.nodeId;
                if(!str.compare(setVoltageBrowseName))
                    setVoltageNodeId=ref.nodeId.nodeId;
                if(!str.compare(setVoltageMaxBrowseName))
                    setVoltageMaxNodeId=ref.nodeId.nodeId;
                if(!str.compare(setRampUpBrowseName))
                    setRampUpNodeId=ref.nodeId.nodeId;
                if(!str.compare(setRampDownBrowseName))
                    setRampDownNodeId=ref.nodeId.nodeId;
            }
    }
}
*/
