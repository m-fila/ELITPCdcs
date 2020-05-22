
#ifndef DCS_N1471_CONTROLLER
#define DCS_N1471_CONTROLLER
#include "DCSDeviceController.h"
#include "N1471.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSN1471Controller : public DCSDeviceController<N1471> {
  friend DCSServer;

private:
  DCSN1471Controller(UA_Server *server, std::string name);

  void setChannel(std::vector<UA_Variant> input, UA_Variant *output);
  void setVoltage(std::vector<UA_Variant> input, UA_Variant *output);
  void setCurrent(std::vector<UA_Variant> input, UA_Variant *output);
  void setVoltageMax(std::vector<UA_Variant> input, UA_Variant *output);
  void setRampUp(std::vector<UA_Variant> input, UA_Variant *output);
  void setRampDown(std::vector<UA_Variant> input, UA_Variant *output);

  UA_DT1415m getMeasurements();
  UA_DT1415c getConfiguration();
};

#endif // DCS_N1471_CONTROLLER
