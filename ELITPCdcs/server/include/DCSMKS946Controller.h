
#ifndef DCS_MKS946_CONTROLLER_H
#define DCS_MKS946_CONTROLLER_H
#include "DCSDeviceController.h"
#include "MKS946.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DCSMKS946Controller : public DCSDeviceController<MKS946>,
                            public DCSObjectFactory::Register<DCSMKS946Controller> {
    friend DCSServer;
    friend DCSObjectFactory;

  private:
    static std::string GetType() { return "MKS946"; }
    DCSMKS946Controller(){};
    void addChildren(const Options &options) override;

    void setUnits(const UA_Variant *input, UA_Variant *output);
    void setRelay(const UA_Variant *input, UA_Variant *output);

    UA_MKS946m getMeasurements();
};

#endif  // DCS_MKS946_CONTROLLER_H