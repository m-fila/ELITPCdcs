
#ifndef DCS_TPG362_CONTROLLER_H
#define DCS_TPG362_CONTROLLER_H
#include "DCSDeviceController.h"
#include "TPG362.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSTPG362Controller : public DCSDeviceController<TPG362> {
  friend DCSServer;

private:
  DCSTPG362Controller(UA_Server *server, std::string name);

  UA_TPG362m getMeasurements();
  UA_TPG362c getConfiguration();
};

#endif // DCS_TPG362_CONTROLLER_H
