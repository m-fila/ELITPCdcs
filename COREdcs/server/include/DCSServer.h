#ifndef DCS_SERVER_H
#define DCS_SERVER_H
#include <map>
#include <memory>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <signal.h>
#include <stdexcept>
#include <string>
#include "DCSWorkerThread.h"
#include "DCSHistoryBackend.h"
#include "DCSObject.h"
#include "DCSLogger.h"
#include "DCSContext.h"

class DCSServer {

public:
  DCSServer(std::string address, int port);
  ~DCSServer();
 std::map<std::string,DCSHistoryBackend*> historyBackends;
  static DCSServer* getServerContext(UA_Server* server){return DCS::getContext<DCSServer*>(server,UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER));}
  DCSHistoryBackend* getHistoryBackend(std::string backendName){
    auto i=historyBackends.find(backendName);
    return i!=historyBackends.end() ? i->second  : nullptr;
  }

  int run();

  template <class T> T &addController(std::string name) {
    if (objects.find(name) == objects.end()) {
      return *static_cast<T *>(
          objects.insert({name, new T(server, name)}).first->second);
    } else {
      throw std::runtime_error("Not unique controller id: " + name);
    }
  }

  DCSObject &addObject(std::string name) {
    return addController<DCSObject>(name);
  }

  template < class T>
  T& addHistoryBackend(std::string name){

   if (historyBackends.find(name) == historyBackends.end()) {
      return *static_cast<T *>(
          historyBackends.insert({name, new T(server)}).first->second);
    } else {
      throw std::runtime_error("Not unique backend id: " + name);
    }
    
  }

  void setDescription(std::string appName, std::string appURI,
                      std::string productURI);

protected:
  std::map<std::string, DCSObject *> objects;
 
  static UA_Boolean running;

  static void stopHandler(int sig) {
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, "Received ctrl-c");
    running = false;
  }

  void addCustomTypes(UA_DataTypeArray *custom);
  
  void addHistorizing();
  UA_Server *server;
  UA_ServerConfig *config;
};

#endif // DCS_SERVER_H
