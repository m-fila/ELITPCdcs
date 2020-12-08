#ifndef PIWEATHER_VARIABLE_H
#define PIWEATHER_VARIABLE_H
#include "open62541/types_elitpcnodeset_generated.h"
#include "template_variable.h"

std::ostream &operator<<(std::ostream &os, const UA_PiWeatherm &data);

class PiWeatherVariable : public template_variable<UA_PiWeatherm> {
  public:
    PiWeatherVariable(std::string ObjectName, std::string VariableName);
    std::string translateKeys() final;
};

#endif  // TPG362_VARIABLE_H
