#ifndef DCS_SERVER_H
#define DCS_SERVER_H
#include "DCSObject.h"
#include <map>
#include <memory>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <stdexcept>
#include <string>

class DCSServer {

public:
  DCSServer(std::string address, std::string port);
  ~DCSServer();

  int run();

  template <class T> T &addController(std::string name) {
    if (objects.find(name) == objects.end()) {
      return *static_cast<T *>(
          objects.insert({name, new T(server, name)}).first->second);
    } else {
      throw std::runtime_error("None unique controller id: " + name);
    }
  }

  DCSObject &addObject(std::string name) {
    return addController<DCSObject>(name);
  }

  void setDescription(std::string appName, std::string appURI,
                      std::string productURI);

protected:
  std::map<std::string, DCSObject *> objects;
  static UA_Boolean running;

  static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received ctrl-c");
    running = false;
  }

  void addCustomTypes(UA_DataTypeArray *custom);

  UA_Server *server;
  UA_ServerConfig *config;
};

#endif // DCS_SERVER_H
