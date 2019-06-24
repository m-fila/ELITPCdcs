#ifndef ABSTRACT_VARIABLE_H
#define ABSTRACT_VARIABLE_H

#include <string>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
class abstract_variable
{
public:
    abstract_variable(std::string name);
    ~abstract_variable();
    const std::string VariableName;
    UA_NodeId VariableId;
    virtual std::string translateValue()=0;
    virtual std::string translateName()=0;
    virtual void addMonitoredItem(UA_Client *client,UA_CreateSubscriptionResponse response);
    static void handler_ValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                                     UA_UInt32 monId, void *monContext, UA_DataValue *value);
    virtual void updateData(UA_DataValue *value)=0;
};

#endif // ABSTRACT_VARIABLE_H
