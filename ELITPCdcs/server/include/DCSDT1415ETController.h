
#ifndef DCS_DT1415ET_CONTROLLER
#define DCS_DT1415ET_CONTROLLER
#include "DCSDeviceController.h"
#include "DT1415ET.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSDT1415ETController : public DCSDeviceController<DT1415ET> {
  friend DCSServer;

private:
  DCSDT1415ETController(UA_Server *server, std::string name);

  void setChannel(std::vector<UA_Variant> input, UA_Variant *output);
  void setVoltage(std::vector<UA_Variant> input, UA_Variant *output);
  void setCurrent(std::vector<UA_Variant> input, UA_Variant *output);
  void setVoltageMax(std::vector<UA_Variant> input, UA_Variant *output);
  void setRampUp(std::vector<UA_Variant> input, UA_Variant *output);
  void setRampDown(std::vector<UA_Variant> input, UA_Variant *output);

  UA_DT1415m getMeasurements();
  UA_DT1415c getConfiguration();
};

#endif // DCS_DT1415ET_CONTROLLER
