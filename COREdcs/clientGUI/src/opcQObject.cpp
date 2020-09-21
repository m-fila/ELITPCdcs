#include "opcQObject.h"

opcQObject::opcQObject(std::string OName, QObject *parent)
    : QObject(parent), ObjectName(OName) {
    ObjectNodeId = UA_NODEID_STRING_ALLOC(1, ObjectName.c_str());
}

opcQObject::~opcQObject() { UA_NodeId_deleteMembers(&ObjectNodeId); }

void opcQObject::opcInit(UA_Client *Client, UA_ClientConfig *Config,
                         UA_CreateSubscriptionResponse response) {
    client = Client;
    config = Config;
}

void opcQObject::addMonitoredItem(UA_NodeId VariableId,
                                  UA_CreateSubscriptionResponse response,
                                  void (*ValueChangedCallback)(UA_Client *, UA_UInt32,
                                                               void *, UA_UInt32, void *,
                                                               UA_DataValue *),
                                  UA_Double sampling) {

    UA_MonitoredItemCreateRequest monRequest =
        UA_MonitoredItemCreateRequest_default(VariableId);
    monRequest.requestedParameters.samplingInterval = sampling;
    UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(
        client, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monRequest, this,
        ValueChangedCallback, nullptr);
}

void opcQObject::browseIds() {
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    bReq.nodesToBrowse[0].nodeId = ObjectNodeId;  // UA_NODEID_NUMERIC(0, UA_NS0ID_OBJE
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_BROWSENAME;
    UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    for(size_t i = 0; i < bResp.resultsSize; ++i) {
        for(size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
            UA_ReferenceDescription ref = (bResp.results[i].references[j]);
            std::string str =
                std::string(reinterpret_cast<char *>(ref.browseName.name.data));
            browsedIds[str] = ref.nodeId.nodeId;
        }
    }
}
