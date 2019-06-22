#ifndef HMP_VARIABLE_H
#define HMP_VARIABLE_H
#include "template_variable.h"
#include "HMPDataType.h"
class hmp_variable : public template_variable<HMPMeasurements>
{
public:
    hmp_variable(std::string name);
    ~hmp_variable();
    virtual std::string translateName();
};

#endif // HMP_VARIABLE_H
