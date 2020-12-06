#ifndef TPG362_VARIABLE_H
#define TPG362_VARIABLE_H
#include "open62541/types_elitpcnodeset_generated.h"
#include "template_variable.h"

std::ostream &operator<<(std::ostream &os, const UA_TPG362m &data);

class TPG362Variable : public template_variable<UA_TPG362m> {
  public:
    TPG362Variable(std::string ObjectName, std::string VariableName);
    std::string translateKeys() final;
};

#endif  // TPG362_VARIABLE_H
