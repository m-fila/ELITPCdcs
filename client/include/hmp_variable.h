#ifndef HMP_VARIABLE_H
#define HMP_VARIABLE_H
#include "template_variable.h"
#include "../../common/HMPDataType.h"
class hmp_variable : public template_variable<HMPMeasurements>
{
public:
    hmp_variable(std::string ObjectName,std::string VariableName);
    ~hmp_variable();
    hmp_customType customType;
    virtual std::string translateKeys();
};

#endif // HMP_VARIABLE_H
