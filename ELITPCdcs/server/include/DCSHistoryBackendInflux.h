#ifndef DCS_HISTORY_BACKEND_INFLUX_H
#define DCS_HISTORY_BACKEND_INFLUX_H
#include "DCSHistoryBackend.h"
#include "DCSLogger.h"
#include "DCSWorkerThread.h"
#include "Influx.h"
#include <json.hpp>
class DCSServer;

class DCSHistoryBackendInflux : public DCSHistoryBackend {
    friend DCSServer;

  public:
    UA_HistoryDataBackend getUaBackend() override;
    void setConnetionAddress(std::string address, int port) {
        db.setConnectionParameters(address, port);
    }
    void setUser(std::string userName, std::string password) {
        db.setUser(userName);
        db.setPassword(password);
    }
    void setDatabase(std::string databaseName) { db.setDatabase(databaseName); }

  private:
    DCSHistoryBackendInflux(UA_Server *server) : DCSHistoryBackend(server, "influxdb") {}

    static UA_StatusCode
    serverSetHistoryData(UA_Server *server, void *context, const UA_NodeId *sessionId,
                         void *sessionContext, const UA_NodeId *nodeId,
                         UA_Boolean historizing, const UA_DataValue *value);

    static size_t resultSize(UA_Server *server, void *context, const UA_NodeId *sessionId,
                             void *sessionContext, const UA_NodeId *nodeId,
                             size_t startIndex, size_t endIndex);

    static size_t getEnd(UA_Server *server, void *context, const UA_NodeId *sessionId,
                         void *sessionContext, const UA_NodeId *nodeId);

    static size_t lastIndex(UA_Server *server, void *context, const UA_NodeId *sessionId,
                            void *sessionContext, const UA_NodeId *nodeId);

    static size_t firstIndex(UA_Server *server, void *context, const UA_NodeId *sessionId,
                             void *sessionContext, const UA_NodeId *nodeId);

    static size_t getDateTimeMatch(UA_Server *server, void *context,
                                   const UA_NodeId *sessionId, void *sessionContext,
                                   const UA_NodeId *nodeId, const UA_DateTime timestamp,
                                   const MatchStrategy strategy);

    static UA_StatusCode copyDataValues(UA_Server *server, void *context,
                                        const UA_NodeId *sessionId, void *sessionContext,
                                        const UA_NodeId *nodeId, size_t startIndex,
                                        size_t endIndex, UA_Boolean reverse,
                                        size_t maxValues, UA_NumericRange range,
                                        UA_Boolean releaseContinuationPoints,
                                        const UA_ByteString *continuationPoint,
                                        UA_ByteString *outContinuationPoint,
                                        size_t *providedValues, UA_DataValue *values);

    static const UA_DataValue *getDataValue(UA_Server *server, void *context,
                                            const UA_NodeId *sessionId,
                                            void *sessionContext, const UA_NodeId *nodeId,
                                            size_t index);

    static UA_Boolean boundSupported(UA_Server *server, void *context,
                                     const UA_NodeId *sessionId, void *sessionContext,
                                     const UA_NodeId *nodeId);
    static UA_Boolean
    timestampsToReturnSupported(UA_Server *server, void *context,
                                const UA_NodeId *sessionId, void *sessionContext,
                                const UA_NodeId *nodeId,
                                const UA_TimestampsToReturn timestampsToReturn);

    static UA_StatusCode insertDataValue(UA_Server *server, void *hdbContext,
                                         const UA_NodeId *sessionId, void *sessionContext,
                                         const UA_NodeId *nodeId,
                                         const UA_DataValue *value);

    static UA_StatusCode updateDataValue(UA_Server *server, void *hdbContext,
                                         const UA_NodeId *sessionId, void *sessionContext,
                                         const UA_NodeId *nodeId,
                                         const UA_DataValue *value);

    static UA_StatusCode replaceDataValue(UA_Server *server, void *hdbContext,
                                          const UA_NodeId *sessionId,
                                          void *sessionContext, const UA_NodeId *nodeId,
                                          const UA_DataValue *value);

    static UA_StatusCode removeDataValue(UA_Server *server, void *hdbContext,
                                         const UA_NodeId *sessionId, void *sessionContext,
                                         const UA_NodeId *nodeId,
                                         UA_DateTime startTimestamp,
                                         UA_DateTime endTimestamp);

    static void deleteMembers(UA_HistoryDataBackend *backend);

    std::string toInflux(nlohmann::json j);

    void write(const std::string &str);

    Influx db;
    DCSWorkerThread worker;
};
#endif  // DCS_HISTORY_BACKEND_INFLUX_H
