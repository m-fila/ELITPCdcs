#ifndef DCS_SERVER_H
#define DCS_SERVER_H
#include "DCSContext.h"
#include "DCSHistoryBackend.h"
#include "DCSLogger.h"
#include "DCSObject.h"
#include "DCSRecovery.h"
#include "DCSWorkerThread.h"
#include "open62541/namespace_dcsnodeset_generated.h"
#include <map>
#include <memory>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <stdexcept>
#include <string>
class DCSServer {

  public:
    /**
     * @brief Construct a new DCSServer object
     *
     * @param address
     * @param port
     */
    DCSServer(std::string address, int port);
    ~DCSServer();

    int run();

    static DCSServer *getServerContext(UA_Server *server) {
        return DCS::getContext<DCSServer *>(server,
                                            UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER));
    }

    DCSHistoryBackend *getHistoryBackend(const std::string &backendName) {
        auto i = historyBackends.find(backendName);
        return i != historyBackends.end() ? i->second : nullptr;
    }

    DCSObject *addObject(std::string typeName, std::string name,
                         const Options &options = {});

    template <class T>
    DCSObject *addObject(std::string name, const Options &options = {});

    template <class T> T &addHistoryBackend(std::string name);
    void setDescription(std::string appName, std::string appURI, std::string productURI);
    void setBuildInfo(std::string productUri, std::string productName, std::string manufacturerName, std::string softwareVersion);

    void setProfileDir(const std::string &path) {
        profileDir = path;
        if(profileDir.back() != '/') {
            profileDir.push_back('/');
        }
    }
    std::string getProfileDir() { return profileDir; }

    std::shared_ptr<DCSRecovery> setRecovery(const std::string &recoveryFileName) {
        recovery = std::make_shared<DCSRecovery>(recoveryFileName);
        return recovery;
    };
    std::shared_ptr<DCSRecovery> getRecovery() { return recovery; }

  protected:
    std::map<std::string, DCSObject *> objects;
    std::map<std::string, DCSHistoryBackend *> historyBackends;

    void addCustomTypes(UA_DataTypeArray *custom);
    void addHistorizing();

    UA_NodeId
    addObjectType(std::string typeName,
                  UA_NodeId parentTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE));

    UA_Server *server;
    UA_ServerConfig *config;

  private:
    static UA_Boolean running;
    static void stopHandler(int sig);

    std::string profileDir;
    DCSWorkerThread dispatcherThread;
    static void asyncCallback(UA_Server *server);

    std::shared_ptr<DCSRecovery> recovery;
};

template <class T>
DCSObject *DCSServer::addObject(std::string name, const Options &options) {
    if(objects.find(name) == objects.end()) {
        auto newObject = new T;
        newObject->init(T::GetType(), name, server, options);
        objects[name] = newObject;
        return newObject;
    } else {
        throw std::runtime_error("Not unique controller id: " + name);
    }
}

template <class T> T &DCSServer::addHistoryBackend(std::string name) {

    if(historyBackends.find(name) == historyBackends.end()) {
        return *static_cast<T *>(
            historyBackends.insert({name, new T(server)}).first->second);
    } else {
        throw std::runtime_error("Not unique backend id: " + name);
    }
}

#endif  // DCS_SERVER_H
