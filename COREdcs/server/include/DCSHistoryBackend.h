#ifndef DCS_HISTORY_BACKEND_H
#define DCS_HISTORY_BACKEND_H
#include <open62541/plugin/historydata/history_data_backend.h>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/server.h>
#include <open62541/server_config.h>
#include <string>
class DCSServer;

typedef struct {
    UA_HistoryDataGathering gathering;
} UA_HistoryDatabaseContext_default;

class DCSHistoryBackend {

    friend DCSServer;

  public:
    virtual ~DCSHistoryBackend() = default;
    virtual UA_HistoryDataBackend getUaBackend() = 0;
    UA_HistoryDataGathering getGathering() {
        return static_cast<UA_HistoryDatabaseContext_default *>(
                   UA_Server_getConfig(server)->historyDatabase.context)
            ->gathering;
    }

  protected:
    DCSHistoryBackend(UA_Server *server, std::string name) : server(server), name(name) {}

    UA_Server *server;
    std::string name;
};
#endif  // DCS_HISTORY_H
