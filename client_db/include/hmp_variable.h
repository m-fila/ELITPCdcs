#ifndef HMP_VARIABLE_H
#define HMP_VARIABLE_H
#include "template_variable.h"
#include "open62541/types_dcsnodeset_generated.h"
std::ostream& operator<<(std::ostream& os, const UA_HMPm& data);

class HMPVariable : public template_variable<UA_HMPm>
{
public:
    HMPVariable(std::string ObjectName,std::string VariableName, int size);
    std::string translateKeys() final;
private:
    const int size;
};

class HMP2020Variable: public HMPVariable{
public:
    HMP2020Variable(std::string ObjectName, std::string VariableName): HMPVariable(ObjectName,VariableName,2){}
};

class HMP4040Variable: public HMPVariable{
public:
    HMP4040Variable(std::string ObjectName, std::string VariableName): HMPVariable(ObjectName,VariableName,4){}
};
#endif // HMP_VARIABLE_H
