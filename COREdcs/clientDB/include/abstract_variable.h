#ifndef ABSTRACT_VARIABLE_H
#define ABSTRACT_VARIABLE_H

#include "database.h"
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include <string>
class abstract_variable {
  public:
    abstract_variable(std::string ObjectName, std::string VariableName);
    ~abstract_variable();
    const std::string ObjectName;
    const std::string VariableName;
    const std::string FullName;
    UA_NodeId VariableNodeId;
    void init(UA_Client *client);
    virtual std::string translateValue() = 0;
    virtual std::string translateKeys() = 0;
    virtual void addMonitoredItem(UA_Client *client,
                                  UA_CreateSubscriptionResponse response);
    static void handler_ValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                                     UA_UInt32 monId, void *monContext,
                                     UA_DataValue *value);
    virtual void updateData(UA_DataValue *value) = 0;
};

#endif  // ABSTRACT_VARIABLE_H
