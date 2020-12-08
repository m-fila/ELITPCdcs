
#ifndef DCS_DT1415ET_CONTROLLER
#define DCS_DT1415ET_CONTROLLER
#include "DCSDeviceController.h"
#include "DT1415ET.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "open62541/types_elitpcnodeset_generated_handling.h"
class DCSDT1415ETController : public DCSDeviceController<DT1415ET>,
                              DCSObjectFactory::Register<DCSDT1415ETController> {
    friend DCSServer;
    friend DCSObjectFactory;

  private:
    DCSDT1415ETController(){};
    static std::string GetType() { return "DT1415ET"; }
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
// REGISTER_OBJECT(DCSDT1415ETController)
#endif  // DCS_DT1415ET_CONTROLLER
