#ifndef OPC_SERVER_H
#define OPC_SERVER_H
#include "opc_monitor.h"
#include <map>
#include <memory>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <signal.h>
#include <stdexcept>
#include <string>
//#include "open62541/namespace_dcsnodeset_generated.h"

class OpcServer {
public:
  OpcServer(std::string address, std::string port);
  ~OpcServer();
  UA_Server *server;
  UA_ServerConfig *config;
  int run();
  template <class T> std::shared_ptr<T> addController(std::string name) {
    if (controllers.find(name) == controllers.end()) {
      auto controller = std::make_shared<T>(name);
      controller->init(server);
      controllers[name]=controller;
      return controller;
    } else {
      throw std::runtime_error("None unique controller id: " + name);
    }
  }

protected:
  std::map<std::string, std::shared_ptr<OpcMonitor>> controllers;
  static UA_Boolean running;
  static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
  }
  void addCustomTypes(UA_DataTypeArray *custom);
};

#endif // OPC_SERVER_H
