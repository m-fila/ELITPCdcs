#ifndef DT1415_VARIABLE_H
#define DT1415_VARIABLE_H
#include "template_variable.h"
#include "open62541/types_dcsnodeset_generated.h"
class dt1415_variable : public template_variable<UA_DT1415m>
{
public:
    dt1415_variable(std::string ObjectName,std::string VariableName);
    ~dt1415_variable();
    std::string translateKeys() final;
private:
    int size=8;
};

#endif // DT1415_VARIABLE_H
