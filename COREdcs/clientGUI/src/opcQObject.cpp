#include "opcQObject.h"
#include <iostream>
opcQObject::opcQObject(std::string OName, QObject *parent)
    : QObject(parent), ObjectName(OName) {
    ObjectNodeId = UA_NODEID_STRING_ALLOC(1, ObjectName.c_str());
}

opcQObject::~opcQObject() {
    UA_NodeId_deleteMembers(&ObjectNodeId);
    for(auto &i : monitoredItems) {
        delete i.second;
    }
}

void opcQObject::opcInit(UA_Client *Client, UA_ClientConfig *Config,
                         UA_CreateSubscriptionResponse response) {
    client = Client;
    config = Config;
    browseIds();
    for(auto &i : monitoredItems) {
        try {
            UA_MonitoredItemCreateRequest monRequest =
                UA_MonitoredItemCreateRequest_default(browsedIds.at(i.first));
            // monRequest.requestedParameters.samplingInterval = sampling;
            UA_Client_MonitoredItems_createDataChange(
                client, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monRequest,
                i.second, &DCSMonitoredItem::callback, nullptr);
        } catch(const std::out_of_range &e) {
            std::cout << ObjectName << "." << i.first
                      << ": creating monitored item failed" << std::endl;
        }
    }
}

DCSMonitoredItem *opcQObject::addMonitoredItem(const std::string &browseName) {
    auto *newItem = new DCSMonitoredItem(browseName, this);
    monitoredItems[browseName] = newItem;
    return newItem;
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
void DCSMonitoredItem::callback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                UA_UInt32 monId, void *monContext, UA_DataValue *value) {
    if(value->hasValue) {
        if(!UA_Variant_isEmpty(&value->value)) {
            auto *context = static_cast<DCSMonitoredItem *>(monContext);
            emit context->valueChanged(value->value);
        }
    }
}
