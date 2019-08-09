#ifndef HMP_VARIABLE_H
#define HMP_VARIABLE_H
#include "template_variable.h"
#include "open62541/types_dcsnodeset_generated.h"
class hmp_variable : public template_variable<UA_HMPm>
{
public:
    hmp_variable(std::string ObjectName,std::string VariableName);
    ~hmp_variable();
    std::string translateKeys() final;
private:
    int size=2;
};

#endif // HMP_VARIABLE_H
