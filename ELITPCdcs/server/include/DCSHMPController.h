
#ifndef DCS_HMP_CONTROLLER_H
#define DCS_HMP_CONTROLLER_H
#include "DCSDeviceController.h"
#include "HMP2020.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSHMPController : public DCSDeviceController<HMP2020> {
  friend DCSServer;

protected:
  DCSHMPController(int size) : size(size) {}
  virtual ~DCSHMPController() {}
  void addChildren(Options options) override;

private:
  void setChannel(const UA_Variant *input, UA_Variant *output);
  void setOutput(const UA_Variant *input, UA_Variant *output);
  void setVoltage(const UA_Variant *input, UA_Variant *output);
  void setCurrent(const UA_Variant *input, UA_Variant *output);
  UA_HMPm getMeasurements();
  UA_HMPc getConfiguration();
  const int size;
};

class DCSHMP2020Controller : public DCSHMPController,
                             DCSObjectFactory::Register<DCSHMP2020Controller> {
  friend DCSServer;
  friend DCSObjectFactory;

private:
  static std::string GetType() { return "HMP2020"; }
  DCSHMP2020Controller() : DCSHMPController(2) {}
};

class DCSHMP4040Controller : public DCSHMPController,
                             DCSObjectFactory::Register<DCSHMP4040Controller> {
  friend DCSServer;
  friend DCSObjectFactory;

private:
  static std::string GetType() { return "HMP4040"; }
  DCSHMP4040Controller() : DCSHMPController(4) {}
};

#endif // DCS_HMP_CONTROLLER_H
