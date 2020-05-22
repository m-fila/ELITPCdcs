
#ifndef DCS_HMP_CONTROLLER_H
#define DCS_HMP_CONTROLLER_H
#include "DCSDeviceController.h"
#include "HMP2020.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSHMPController : public DCSDeviceController<HMP2020> {
  friend DCSServer;

protected:
  DCSHMPController(UA_Server *server, std::string name, int size);

private:
  void setChannel(std::vector<UA_Variant> input, UA_Variant *output);
  void setOutput(std::vector<UA_Variant> input, UA_Variant *output);
  void setVoltage(std::vector<UA_Variant> input, UA_Variant *output);
  void setCurrent(std::vector<UA_Variant> input, UA_Variant *output);
  UA_HMPm getMeasurements();
  UA_HMPc getConfiguration();
  const int size;
};

class DCSHMP2020Controller : public DCSHMPController {
  friend DCSServer;

private:
  DCSHMP2020Controller(UA_Server *server, std::string name)
      : DCSHMPController(server, name, 2) {}
};

class DCSHMP4040Controller : public DCSHMPController {
  friend DCSServer;

private:
  DCSHMP4040Controller(UA_Server *server, std::string name)
      : DCSHMPController(server, name, 4) {}
};
#endif // DCS_HMP_CONTROLLER_H
