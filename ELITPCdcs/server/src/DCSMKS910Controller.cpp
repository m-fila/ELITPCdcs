#include "DCSMKS910Controller.h"
void DCSMKS910Controller::addChildren(const Options &options) {
    addConnection();
    auto &m =
        addVariable("measurements", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910M]);
    addVariableUpdate(m, 1000, &DCSMKS910Controller::getMeasurements, this);
    m.setHistorizing();
    auto &c =
        addVariable("configuration", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910C]);
    addVariableUpdate(c, 1000, &DCSMKS910Controller::getConfiguration, this);

    auto &r = addVariable("relay", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_RELAY]);
    addVariableUpdate(r, 5000, &DCSMKS910Controller::getRelay, this);
    addControllerMethod("setunits", "Sets pressure units",
                        {{"Unit", "PASCAL/BAR/TORR", UA_TYPES[UA_TYPES_INT16]}}, {},
                        &DCSMKS910Controller::setUnits, this);
    addControllerMethod("setrelay", "Sets relay",
                        {{"Relay number", "1-3", UA_TYPES[UA_TYPES_UINT32]},
                         {"Enabled", "OFF/ON", UA_TYPES[UA_TYPES_UINT32]},
                         {"Setpoint", "Current units", UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Hysteresis", "Current units", UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSMKS910Controller::setRelay, this);
}

UA_MKS910m DCSMKS910Controller::getMeasurements() {
    UA_MKS910m mks;
    UA_MKS910m_init(&mks);
    mks.combined = std::stod(device.getPCombinedLong());
    mks.piezo = std::stod(device.getPPirani());
    mks.pirani = std::stod(device.getPPirani());
    mks.temperature = std::stod(device.getTemp());
    mks.status = static_cast<int>(MKS910codes::statusFromString.at(device.getStatus()));
    mks.units = static_cast<int>(MKS910codes::unitsFromString.at(device.getUnits()));
    return mks;
}

UA_MKS910c DCSMKS910Controller::getConfiguration() {
    UA_MKS910c mks;
    UA_MKS910c_init(&mks);
    return mks;
}

void DCSMKS910Controller::setUnits(const UA_Variant *input, UA_Variant *) {
    int unit = *static_cast<UA_Int16 *>(input[0].data);
    device.setUnits(static_cast<MKS910codes::Units>(unit));
}

UA_Relay DCSMKS910Controller::getRelay() {
    size_t size = 3;
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
        auto no = static_cast<MKS910::RelayNo>(i + 1);
        std::string resp;
        resp = device.getRelayDirection(no);
        relay.direction[i] =
            static_cast<int>(MKS910codes::relayDirectionFromString.at(resp));
        resp = device.getRelayEnabled(no);
        relay.enabled[i] = static_cast<int>(MKS910codes::relayEnabledFromString.at(resp));
        relay.hysteresis[i] = std::stod(device.getRelayHysteresis(no));
        relay.setpoint[i] = std::stod(device.getRelayHysteresis(no));
        resp = device.getRelayStatus(no);
        relay.status[i] = static_cast<int>(MKS910codes::relaySetFromString.at(resp));
    }
    return relay;
}

void DCSMKS910Controller::setRelay(const UA_Variant *input, UA_Variant *output) {
    auto no = *static_cast<UA_UInt32 *>(input[0].data);
    auto relayNo = static_cast<MKS910::RelayNo>(no);
    auto enabled = *static_cast<UA_UInt32 *>(input[1].data);
    auto setpoint = *static_cast<UA_Double *>(input[2].data);
    auto hysteresis = *static_cast<UA_Double *>(input[3].data);

    device.setRelayEnabled(relayNo, false);
    if(setpoint < hysteresis) {
        device.setRelayDirection(relayNo, MKS910codes::RelayDirection::Below);
    } else {
        device.setRelayDirection(relayNo, MKS910codes::RelayDirection::Above);
    }
    device.setRelaySetpoint(relayNo, setpoint);
    device.setRelayHysteresis(relayNo, hysteresis);
    if(enabled) {
        device.setRelayEnabled(relayNo, enabled);
    }
}
