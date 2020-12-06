#include "DCSMKS946Controller.h"
void DCSMKS946Controller::addChildren(const Options &options) {
    addConnection(options);
    auto &m = addVariable("measurements",
                          UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariable("configuration", UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariableUpdate(m, 1000, &DCSMKS946Controller::getMeasurements, this);
    m.setHistorizing();
}

UA_MKS946m DCSMKS946Controller::getMeasurements() {
    UA_MKS946m mks;
    UA_MKS946m_init(&mks);
    mks.flow = std::stod(device.getFlow(MKS946::CH::CH1));
    mks.pressure = std::stod(device.getPressure(MKS946::CH::CH3));
    return mks;
}
