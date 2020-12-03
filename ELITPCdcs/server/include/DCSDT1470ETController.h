
#ifndef DCS_DT1470ET_CONTROLLER
#define DCS_DT1470ET_CONTROLLER
#include "DCSDeviceController.h"
#include "DT1470ET.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSDT1470ETController : public DCSDeviceController<DT1470ET>,
                              DCSObjectFactory::Register<DCSDT1470ETController> {
    friend DCSServer;
    friend DCSObjectFactory;

  private:
    DCSDT1470ETController(){};
    static std::string GetType() { return "DT1470ET"; }
    void addChildren(const Options &options) final;

    void setChannel(const UA_Variant *input, UA_Variant *output);
    void setVoltage(const UA_Variant *input, UA_Variant *output);
    void setCurrent(const UA_Variant *input, UA_Variant *output);
    void setVoltageMax(const UA_Variant *input, UA_Variant *output);
    void setRampUp(const UA_Variant *input, UA_Variant *output);
    void setRampDown(const UA_Variant *input, UA_Variant *output);

    UA_DT1415m getMeasurements();
    UA_DT1415c getConfiguration();
};
// REGISTER_OBJECT(DCSDT1470ETController)
#endif  // DCS_DT1470ET_CONTROLLER
