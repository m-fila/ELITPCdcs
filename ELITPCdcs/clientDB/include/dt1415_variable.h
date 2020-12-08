#ifndef DT1415_VARIABLE_H
#define DT1415_VARIABLE_H
#include "open62541/types_elitpcnodeset_generated.h"
#include "template_variable.h"
std::ostream &operator<<(std::ostream &os, const UA_DT1415m &data);

class caen_variable : public template_variable<UA_DT1415m> {
  public:
    caen_variable(std::string ObjectName, std::string VariableName, int size);
    ~caen_variable();
    std::string translateKeys() final;

  private:
    int size;
};

class dt1415_variable : public caen_variable {
  public:
    dt1415_variable(std::string ObjectName, std::string VariableName)
        : caen_variable(ObjectName, VariableName, 8) {}
};

class n1471_variable : public caen_variable {
  public:
    n1471_variable(std::string ObjectName, std::string VariableName)
        : caen_variable(ObjectName, VariableName, 4) {}
};

#endif  // DT1415_VARIABLE_H
