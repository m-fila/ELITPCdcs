#include "DCSMKS946Controller.h"
void DCSMKS946Controller::addChildren(const Options &options) {
    DCSDeviceController<MKS946>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariable("configuration", &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariableUpdate(m, 1000, &DCSMKS946Controller::getMeasurements, this);
    m.setHistorizing();
}

UA_MKS946m DCSMKS946Controller::getMeasurements() {
    UA_MKS946m mks;
    UA_MKS946m_init(&mks);
    auto flowCH = MKS946::CH::CH1;
    auto pressureCH = MKS946::CH::CH3;
    mks.flow = std::stod(device.getFlow(flowCH));
    mks.pressure = std::stod(device.getPressure(pressureCH));
    mks.flowSetPoint = std::stod(device.getFlowSetPoint(flowCH));
    mks.flowMode =
        static_cast<int>(MKS946codes::modeFromString.at(device.getFlowMode(flowCH)));
    return mks;
}
