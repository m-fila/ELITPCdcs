#include "DCSMKS946Controller.h"
void DCSMKS946Controller::addChildren(const Options &options) {
    DCSDeviceController<MKS946>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariableUpdate(m, 1000, &DCSMKS946Controller::getMeasurements, this, options);
    m.setHistorizing();
    // addVariable("configuration",
    // &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946C]);
    auto &r = addVariable("relay", &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_RELAY]);
    addVariableUpdate(r, 1000, &DCSMKS946Controller::getRelay, this, options);
    addControllerMethod("setrelay", "Sets relay",
                        {{"Relay number", "1-3", &UA_TYPES[UA_TYPES_UINT32]},
                         {"Enabled", "OFF/ON", &UA_TYPES[UA_TYPES_UINT32]},
                         {"Setpoint", "Current units", &UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Hysteresis", "Current units", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSMKS946Controller::setRelay, this);
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
        static_cast<int>(MKS946codes::flowModeFromString.at(device.getFlowMode(flowCH)));
    return mks;
}

UA_Relay DCSMKS946Controller::getRelay() {
    size_t size = 2;
    UA_Relay relay;
    UA_Relay_init(&relay);
    relay.directionSize = size;
    relay.enabledSize = size;
    relay.hysteresisSize = size;
    relay.setpointSize = size;
    relay.statusSize = size;

    relay.direction =
        static_cast<UA_Boolean *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_BOOLEAN]));
    relay.enabled =
        static_cast<UA_UInt32 *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    relay.hysteresis =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    relay.setpoint =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    relay.status =
        static_cast<UA_Boolean *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_BOOLEAN]));

    for(size_t i = 0; i < size; ++i) {
        // auto no = static_cast<MKS910::RelayNo>(i + 1);
        auto no = static_cast<MKS946::RelayNo>(i + 5);
        std::string resp;
        resp = device.getRelayDirection(no);
        relay.direction[i] =
            static_cast<int>(MKS946codes::relayDirectionFromString.at(resp));
        resp = device.getRelayEnabled(no);
        relay.enabled[i] = static_cast<int>(MKS946codes::relayEnabledFromString.at(resp));
        relay.hysteresis[i] = std::stod(device.getRelayHysteresis(no));
        relay.setpoint[i] = std::stod(device.getRelaySetPoint(no));
        resp = device.getRelayStatus(no);
        relay.status[i] = static_cast<int>(MKS946codes::relaySetFromString.at(resp));
    }
    return relay;
}

void DCSMKS946Controller::setRelay(const UA_Variant *input, UA_Variant *output) {
    auto no = *static_cast<UA_UInt32 *>(input[0].data);
    auto relayNo = static_cast<MKS946::RelayNo>(no);
    auto enabled = *static_cast<UA_UInt32 *>(input[1].data);
    auto setpoint = *static_cast<UA_Double *>(input[2].data);
    auto hysteresis = *static_cast<UA_Double *>(input[3].data);

    device.setRelayEnabled(relayNo, MKS946codes::RelayEnabled::Off);
    if(setpoint < hysteresis) {
        device.setRelayDirection(relayNo, MKS946codes::RelayDirection::Below);
    } else {
        device.setRelayDirection(relayNo, MKS946codes::RelayDirection::Above);
    }
    device.setRelaySetpoint(relayNo, setpoint);
    device.setRelayHysteresis(relayNo, hysteresis);
    device.setRelayEnabled(relayNo, static_cast<MKS946codes::RelayEnabled>(enabled));
}