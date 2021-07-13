
#ifndef DCS_MKS946_CONTROLLER_H
#define DCS_MKS946_CONTROLLER_H
#include "DCSDeviceController.h"
#include "MKS946.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "open62541/types_elitpcnodeset_generated_handling.h"
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
    void configurePID(const UA_Variant *input, UA_Variant *output);
    void configureFlow(const UA_Variant *input, UA_Variant *output);
    void configurePressure(const UA_Variant *input, UA_Variant *output);
    void setPIDState(const UA_Variant *input, UA_Variant *output);

    void addInterlock(const Options &options);
    void configureInterlock(const UA_Variant *input, UA_Variant *output);
    bool interlockAction();

    void postConnect() override;

    UA_MKS946m getMeasurements();
    UA_Relay getRelay();
    UA_MKS946c getConfiguration();
    UA_PID getPID();
    UA_Boolean getPIDState();
    MKS946::CH flowCH = MKS946::CH::CH1;
    MKS946::CH pressureCH = MKS946::CH::CH3;
};

#endif  // DCS_MKS946_CONTROLLER_H
