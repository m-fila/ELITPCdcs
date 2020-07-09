
#ifndef DCS_MKS910_CONTROLLER_H
#define DCS_MKS910_CONTROLLER_H
#include "DCSDeviceController.h"
#include "MKS910.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSMKS910Controller : public DCSDeviceController<MKS910>,
                            public DCSObjectFactory::Register<DCSMKS910Controller> {
    friend DCSServer;
    friend DCSObjectFactory;

  private:
    static std::string GetType() { return "MKS910"; }
    DCSMKS910Controller(){};
    void addChildren(const Options &options) override;

    void setUnits(const UA_Variant *input, UA_Variant *output);
    void setRelay(const UA_Variant *input, UA_Variant *output);

    UA_MKS910m getMeasurements();
    UA_MKS910c getConfiguration();
    UA_Relay getRelay();
};

#endif  // DCS_MKS910_CONTROLLER_H
