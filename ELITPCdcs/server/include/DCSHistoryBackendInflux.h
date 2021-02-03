#ifndef DCS_HISTORY_BACKEND_INFLUX_H
#define DCS_HISTORY_BACKEND_INFLUX_H
#include "DCSHistoryBackend.h"
#include "DCSLogger.h"
#include "DCSWorkerThread.h"
#include "Influx.h"
#include <json.hpp>
#include <mutex>
class DCSServer;

/**
 * @brief Interface for historizing collected data in InfluxDb database.
 * This class doesn't start InfluxDb, only accesses it - you have to setup and start
 * InfluxDb externally
 *
 */
class DCSHistoryBackendInflux : public DCSHistoryBackend {
    friend DCSServer;

  public:
    /**
     * @brief Get the open62541 UA_HistoryData_Backend object
     *
     * @return UA_HistoryDataBackend open62541 struct populated with pointers to this
     */
    UA_HistoryDataBackend getUaBackend() override;
    /**
     * @brief Set the address of InfluxDb
     *
     * @param address ip address, eg. localhost, 127.0.0.1, 192.168.168.2
     * @param port port, eg. 8086
     */
    void setConnetionAddress(std::string address, int port) {
        db.setConnectionParameters(address, port);
    }
    /**
     * @brief Set the username and password for accessing InfluxDb
     *
     * @param userName
     * @param password
     */
    void setUser(std::string userName, std::string password) {
        db.setUser(userName);
        db.setPassword(password);
    }
    /**
     * @brief Set the name of database in InfluxDb
     *
     * @param databaseName
     */
    void setDatabase(std::string databaseName) { db.setDatabase(databaseName); }
    /**
     * @brief Set the time interval at which points will be uploaded to database.
     * According to Influxdb documentation sending multiple points in one request instead
     * of multiple requests with one point is the prefered method.
     *
     * @param interval_ms interval in ms
     */
    void setInterval(size_t interval_ms = 2000) { this->interval_ms = interval_ms; }

  private:
    DCSHistoryBackendInflux(UA_Server *server) : DCSHistoryBackend(server, "influxdb") {
        write();
    }

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

    static std::string toInflux(nlohmann::json j);

    void write();
    static void writeCallback(UA_Server *server, void *context);

    std::mutex bucketMutex;
    std::string bucket;
    size_t interval_ms = 2000;

    Influx db;
    DCSWorkerThread worker;
};
#endif  // DCS_HISTORY_BACKEND_INFLUX_H
