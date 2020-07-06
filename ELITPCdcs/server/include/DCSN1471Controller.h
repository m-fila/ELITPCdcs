
#ifndef DCS_N1471_CONTROLLER
#define DCS_N1471_CONTROLLER
#include "DCSDeviceController.h"
#include "N1471.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSN1471Controller : public DCSDeviceController<N1471>,
                           DCSObjectFactory::Register<DCSN1471Controller> {
  friend DCSServer;
  friend DCSObjectFactory;

private:
  void addChildren(Options options) override;
  DCSN1471Controller(){};
  static std::string GetType() { return "N1471"; }

  void setChannel(const UA_Variant *input, UA_Variant *output);
  void setVoltage(const UA_Variant *input, UA_Variant *output);
  void setCurrent(const UA_Variant *input, UA_Variant *output);
  void setVoltageMax(const UA_Variant *input, UA_Variant *output);
  void setRampUp(const UA_Variant *input, UA_Variant *output);
  void setRampDown(const UA_Variant *input, UA_Variant *output);

  UA_DT1415m getMeasurements();
  UA_DT1415c getConfiguration();
};

#endif // DCS_N1471_CONTROLLER
