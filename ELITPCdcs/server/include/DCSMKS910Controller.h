
#ifndef DCS_MKS910_CONTROLLER_H
#define DCS_MKS910_CONTROLLER_H
#include "DCSDeviceController.h"
#include "MKS910.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSMKS910Controller : public DCSDeviceController<MKS910> {
  friend DCSServer;

private:
  DCSMKS910Controller(UA_Server *server, std::string name);

  void setUnits(std::vector<UA_Variant> input, UA_Variant *output);
  void setRelay(std::vector<UA_Variant> input, UA_Variant *output);
  
  UA_MKS910m getMeasurements();
  UA_MKS910c getConfiguration();
  UA_Relay getRelay();
};

#endif // DCS_MKS910_CONTROLLER_H
