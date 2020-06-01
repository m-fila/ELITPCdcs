#ifndef DCS_OBJECT_H
#define DCS_OBJECT_H
#include "DCSVariable.h"
#include <map>
#include <unordered_map>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
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

struct NodeIdCmp {
    bool operator()(const UA_NodeId& a, const UA_NodeId& b) const {
        return UA_NodeId_hash(&a) < UA_NodeId_hash(&b);
    }
};

class DCSObject {
  friend DCSServer;

public:
  DCSVariable &addVariable(std::string variableName, UA_DataType variableType);

  void addMethod(std::string methodName, std::string methodDescription,
            std::vector<methodArgs> inputs, std::vector<methodArgs> outputs,
            const std::function<void(std::vector<UA_Variant> , UA_Variant *)> &
                methodBody, void * context=nullptr);

protected:
  DCSObject(UA_Server *server, std::string name);
  virtual ~DCSObject();
  UA_Server *server;
  const std::string objectName;
  UA_NodeId objectNodeId;
  std::map<const std::string, DCSVariable> variables;
  std::map<UA_NodeId,
           std::function<void(std::vector<UA_Variant>, UA_Variant *)>,NodeIdCmp>
      methods;
  static UA_StatusCode
  methodCallback(UA_Server *server, const UA_NodeId *sessionId,
                 void *sessionContext, const UA_NodeId *methodId,
                 void *methodContext, const UA_NodeId *objectId,
                 void *objectContext, size_t inputSize, const UA_Variant *input,
                 size_t outputSize, UA_Variant *output);
};

#endif // DCS_OBJECT_H
