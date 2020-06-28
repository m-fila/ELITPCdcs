
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

  void setChannel(const UA_Variant* input, UA_Variant *output);
  void setVoltage(const UA_Variant* input, UA_Variant *output);
  void setCurrent(const UA_Variant* input, UA_Variant *output);
  void setVoltageMax(const UA_Variant* input, UA_Variant *output);
  void setRampUp(const UA_Variant* input, UA_Variant *output);
  void setRampDown(const UA_Variant* input, UA_Variant *output);

  UA_DT1415m getMeasurements();
  UA_DT1415c getConfiguration();
};

#endif // DCS_DT1415ET_CONTROLLER
