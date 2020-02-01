#ifndef PIWEATHER_VARIABLE_H
#define PIWEATHER_VARIABLE_H
#include "template_variable.h"
#include "open62541/types_dcsnodeset_generated.h"

std::ostream& operator<<(std::ostream& os, const UA_PiWeatherm& data);

class PiWeatherVariable : public template_variable<UA_PiWeatherm>
{
public:
    PiWeatherVariable(std::string ObjectName,std::string VariableName);
    std::string translateKeys() final;
};

#endif // TPG362_VARIABLE_H
