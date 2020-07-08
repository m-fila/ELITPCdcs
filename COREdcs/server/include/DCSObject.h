#ifndef DCS_OBJECT_H
#define DCS_OBJECT_H
#include "DCSEvent.h"
#include "DCSFactory.h"
#include "DCSVariable.h"
#include "json.hpp"
#include <functional>
#include <map>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <string>
#include <unordered_map>
#include <vector>

class DCSServer;
class DCSVariable;
class DCSObject;
typedef const nlohmann::json &Options;
typedef DCSFactory<DCSObject, std::string, UA_Server *, Options>
    DCSObjectFactory;

class DCSObject {
  friend DCSServer;
  friend DCSObjectFactory;

public:
  struct Method {
    std::function<void(const UA_Variant *, UA_Variant *)> method;
    std::vector<std::string> inputsNames;
    std::vector<std::string> outputsNames;
    void *context;
    void operator()(const UA_Variant *in, UA_Variant *out) { method(in, out); }
  };
  /* name ,description, UA_DataType
 examples:
    "Status", "ON/OFF", UA_TYPES[UA_TYPES_BOOLEAN]
    "Voltage", "Voltage in V", UA_TYPES[UA_TYPES_DOUBLE]
    "Channel", "Device channel", UA_TYPES[UA_TYPES_UINT16]
*/
  struct MethodArgs {
    std::string name;
    std::string description;
    UA_DataType dataType;
  };

  std::string getName() { return objectName; }
  std::string getType() { return objectType; }

  DCSVariable &addVariable(std::string variableName, UA_DataType variableType);

  Method &addMethod(
      std::string methodName, std::string methodDescription,
      std::vector<MethodArgs> inputs, std::vector<MethodArgs> outputs,
      const std::function<void(const UA_Variant *, UA_Variant *)> &methodBody,
      void *context = nullptr);

  DCSEvent createEvent() { return DCSEvent(server, objectNodeId); }

  void fastEvent(const std::string &sourceName, uint severity,
                 const std::string &message);

protected:
  DCSObject() {}
  virtual ~DCSObject();
  static std::string GetType() { return "DCSObject"; }

  virtual void addChildren(Options options) {}

  UA_Server *server;
  std::string objectName;
  std::string objectType;
  UA_NodeId objectNodeId;

  std::map<const std::string, DCSVariable *> variables;
  std::map<std::string, Method> methods;

private:
  void init(std::string objectType, std::string name, UA_Server *server,
            Options options = {});

  bool addObjectNode();

  static UA_StatusCode
  methodCallback(UA_Server *server, const UA_NodeId *sessionId,
                 void *sessionContext, const UA_NodeId *methodId,
                 void *methodContext, const UA_NodeId *objectId,
                 void *objectContext, size_t inputSize, const UA_Variant *input,
                 size_t outputSize, UA_Variant *output);
};

#endif // DCS_OBJECT_H
