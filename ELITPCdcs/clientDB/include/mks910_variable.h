#ifndef MKS910_VARIABLE_H
#define MKS910_VARIABLE_H
#include "open62541/types_dcsnodeset_generated.h"
#include "template_variable.h"

std::ostream &operator<<(std::ostream &os, const UA_MKS910m &data);

class MKS910Variable : public template_variable<UA_MKS910m> {
  public:
    MKS910Variable(std::string ObjectName, std::string VariableName);
    std::string translateKeys() final;
};

#endif  // TPG362_VARIABLE_H
