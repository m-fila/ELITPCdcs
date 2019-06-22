#ifndef ABSTRACT_VARIABLE_H
#define ABSTRACT_VARIABLE_H

#include <string>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
class abstract_variable
{
public:
    abstract_variable(std::string name);
    ~abstract_variable();
    const std::string VariableName;
    UA_NodeId VariableId;
    virtual std::string translateValue()=0;
    virtual std::string translateName()=0;
    virtual void addSubsciption(){}
};

#endif // ABSTRACT_VARIABLE_H
