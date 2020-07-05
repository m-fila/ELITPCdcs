#ifndef DCS_OBJECT_H
#define DCS_OBJECT_H
#include "DCSEvent.h"
#include "DCSNodeIdMap.h"
#include "DCSVariable.h"
#include <functional>
#include <map>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <string>
#include <unordered_map>
#include <vector>
// dataType, name ,description
// example:  "Status", "ON/OFF", UA_TYPES[UA_TYPES_BOOLEAN]
struct methodArgs {
  std::string name;
  std::string description;
  UA_DataType dataType;
};
class DCSServer;
class DCSVariable;

class DCSObject {
  friend DCSServer;

public:
  struct Method {
    std::function<void(const UA_Variant *, UA_Variant *)> method;
    std::vector<std::string> inputsNames;
    std::vector<std::string> outputsNames;
    void *context;
    void operator()(const UA_Variant *in, UA_Variant *out) { method(in, out); }
  };
  DCSVariable &addVariable(std::string variableName, UA_DataType variableType);
  std::string getName() { return objectName; }
  Method &addMethod(
      std::string methodName, std::string methodDescription,
      std::vector<methodArgs> inputs, std::vector<methodArgs> outputs,
      const std::function<void(const UA_Variant *, UA_Variant *)> &methodBody,
      void *context = nullptr);

  DCSEvent createEvent() { return DCSEvent(server, objectNodeId); }

  void fastEvent(const std::string &sourceName, uint severity,
                 const std::string &message) {
    auto event = createEvent();
    event.setSourceName(sourceName);
    event.setMessage(message);
    event.setSeverity(severity);
    event.trigger();
  }

protected:
  DCSObject(UA_Server *server, std::string name);
  virtual ~DCSObject();
  UA_Server *server;
  const std::string objectName;
  UA_NodeId objectNodeId;
  std::map<const std::string, DCSVariable *> variables;

  std::map<std::string, Method> methods;
  static UA_StatusCode
  methodCallback(UA_Server *server, const UA_NodeId *sessionId,
                 void *sessionContext, const UA_NodeId *methodId,
                 void *methodContext, const UA_NodeId *objectId,
                 void *objectContext, size_t inputSize, const UA_Variant *input,
                 size_t outputSize, UA_Variant *output);
};

#endif // DCS_OBJECT_H
