#include "DCSHistoryBackendInflux.h"
#include "DCSContext.h"
#include "DCSUAJson.h"
#include "DCSVariable.h"
using namespace nlohmann;

UA_HistoryDataBackend DCSHistoryBackendInflux::getUaBackend() {
    UA_HistoryDataBackend result;

    memset(&result, 0, sizeof(UA_HistoryDataBackend));
    result.serverSetHistoryData = &DCSHistoryBackendInflux::serverSetHistoryData;
    result.resultSize = nullptr;        //&DCSHistoryBackendInflux::resultSize;
    result.getEnd = nullptr;            //&DCSHistoryBackendInflux::getEnd;
    result.lastIndex = nullptr;         //&DCSHistoryBackendInflux::lastIndex;
    result.firstIndex = nullptr;        //&DCSHistoryBackendInflux::firstIndex;
    result.getDateTimeMatch = nullptr;  //&DCSHistoryBackendInflux::getDateTimeMatch;
    result.copyDataValues = nullptr;    //&DCSHistoryBackendInflux::copyDataValues;
    result.getDataValue = nullptr;      //&DCSHistoryBackendInflux::getDataValue;
    result.boundSupported = nullptr;    //&DCSHistoryBackendInflux::boundSupported;
    result.timestampsToReturnSupported =
        nullptr;  //&DCSHistoryBackendInflux::timestampsToReturnSupported;
    result.insertDataValue = nullptr;   //&DCSHistoryBackendInflux::insertDataValue;
    result.updateDataValue = nullptr;   //&DCSHistoryBackendInflux::updateDataValue;
    result.replaceDataValue = nullptr;  //&DCSHistoryBackendInflux::replaceDataValue;
    result.removeDataValue = nullptr;   //&DCSHistoryBackendInflux::removeDataValue;
    result.deleteMembers = nullptr;     //&DCSHistoryBackendInflux::deleteMembers;
    result.getHistoryData = nullptr;    //&DCSHistoryBackendInflux::getHistoryData;
    result.context = this;
    return result;
}

UA_StatusCode DCSHistoryBackendInflux::serverSetHistoryData(
    UA_Server *server, void *context, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, UA_Boolean historizing, const UA_DataValue *value) {
    if(!(value->hasValue)) {
        return UA_STATUSCODE_GOODDATAIGNORED;
    } else if(UA_Variant_isEmpty(&value->value)) {
        return UA_STATUSCODE_GOODDATAIGNORED;
    }
    auto instance = static_cast<DCSHistoryBackendInflux *>(context);
    json j = json::parse(DCSUAJson::toString(&value->value, &UA_TYPES[UA_TYPES_VARIANT]));
    size_t time = 0;
    if(value->hasSourceTimestamp) {
        time = value->sourceTimestamp;
    } else if(value->hasServerTimestamp) {
        time = value->serverTimestamp;
    }
    auto cmd = instance->toInflux(j.at("Body"));
    std::string measurement;
    try {
        measurement = DCS::getContext<DCSVariable *>(server, *nodeId)->getFullName();
    } catch(const std::runtime_error &e) {
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    std::unique_lock<std::mutex> lock(instance->bucketMutex);
    instance->bucket.append(
        measurement + " " + cmd + " " +
        (time ? std::to_string((time - UA_DATETIME_UNIX_EPOCH) / UA_DATETIME_MSEC) : "") +
        "\n");
    return UA_STATUSCODE_GOOD;
}

size_t DCSHistoryBackendInflux::resultSize(UA_Server *server, void *context,
                                           const UA_NodeId *sessionId,
                                           void *sessionContext, const UA_NodeId *nodeId,
                                           size_t startIndex, size_t endIndex) {
    return 0;
}

size_t DCSHistoryBackendInflux::getEnd(UA_Server *server, void *context,
                                       const UA_NodeId *sessionId, void *sessionContext,
                                       const UA_NodeId *nodeId) {
    return 0;
}

size_t DCSHistoryBackendInflux::lastIndex(UA_Server *server, void *context,
                                          const UA_NodeId *sessionId,
                                          void *sessionContext, const UA_NodeId *nodeId) {
    return 0;
}

size_t DCSHistoryBackendInflux::firstIndex(UA_Server *server, void *context,
                                           const UA_NodeId *sessionId,
                                           void *sessionContext,
                                           const UA_NodeId *nodeId) {
    return 0;
}

size_t DCSHistoryBackendInflux::getDateTimeMatch(
    UA_Server *server, void *context, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, const UA_DateTime timestamp, const MatchStrategy strategy) {
    return 0;
}

UA_StatusCode DCSHistoryBackendInflux::copyDataValues(
    UA_Server *server, void *context, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, size_t startIndex, size_t endIndex, UA_Boolean reverse,
    size_t maxValues, UA_NumericRange range, UA_Boolean releaseContinuationPoints,
    const UA_ByteString *continuationPoint, UA_ByteString *outContinuationPoint,
    size_t *providedValues, UA_DataValue *values) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

const UA_DataValue *
DCSHistoryBackendInflux::getDataValue(UA_Server *server, void *context,
                                      const UA_NodeId *sessionId, void *sessionContext,
                                      const UA_NodeId *nodeId, size_t index) {
    return UA_DataValue_new();
}

UA_Boolean DCSHistoryBackendInflux::boundSupported(UA_Server *server, void *context,
                                                   const UA_NodeId *sessionId,
                                                   void *sessionContext,
                                                   const UA_NodeId *nodeId) {
    return false;
}
UA_Boolean DCSHistoryBackendInflux::timestampsToReturnSupported(
    UA_Server *server, void *context, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, const UA_TimestampsToReturn timestampsToReturn) {
    return false;
}

UA_StatusCode DCSHistoryBackendInflux::insertDataValue(
    UA_Server *server, void *hdbContext, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, const UA_DataValue *value) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_StatusCode DCSHistoryBackendInflux::updateDataValue(
    UA_Server *server, void *hdbContext, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, const UA_DataValue *value) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_StatusCode DCSHistoryBackendInflux::replaceDataValue(
    UA_Server *server, void *hdbContext, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, const UA_DataValue *value) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_StatusCode DCSHistoryBackendInflux::removeDataValue(
    UA_Server *server, void *hdbContext, const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *nodeId, UA_DateTime startTimestamp, UA_DateTime endTimestamp) {
    return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

void DCSHistoryBackendInflux::deleteMembers(UA_HistoryDataBackend *backend) {}

std::string DCSHistoryBackendInflux::toInflux(json j) {
    if(j.contains("Body")) {
        j = j.at("Body");
    }
    std::stringstream ss;
    for(auto &it : j.items()) {
        auto k = it.key();
        if(k.empty()) {
            k = "v";
        }
        auto v = it.value();
        if(v.is_array()) {
            for(size_t i = 0; i < v.size(); ++i) {
                ss << k << i + 1 << "=" << v.at(i) << ",";
            }
        } else {
            ss << k << "=" << v << ",";
        }
    }
    auto s = ss.str();
    s.pop_back();
    return s;
}

void DCSHistoryBackendInflux::write() {
    std::string messages;
    {
        std::unique_lock<std::mutex> lock(bucketMutex);
        messages = bucket;
        bucket.clear();
    }
    worker.push_back([this, messages]() mutable {
        if(!messages.empty()) {
            messages.pop_back();
            try {
                const auto &response = db.write(messages);
                if(response.statusCode / 100 != 2) {  // status not ok -> code !=2xx
                    UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_NETWORK,
                                   "Influxdb HTTP error, reponse: %i %s",
                                   response.statusCode, response.statusPhrase.c_str());
                }
            } catch(std::runtime_error &e) {
                UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_NETWORK,
                               "Influxdb error: %s", e.what());
            }
        }
        UA_Server_addTimedCallback(
            server, writeCallback, this,
            UA_DateTime_nowMonotonic() + interval_ms * UA_DATETIME_MSEC, nullptr);
    });
}

void DCSHistoryBackendInflux::writeCallback(UA_Server *server, void *context) {
    auto *backend = static_cast<DCSHistoryBackendInflux *>(context);
    backend->write();
}
