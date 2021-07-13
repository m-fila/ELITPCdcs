#include "DCSMKS946Controller.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
void DCSMKS946Controller::addChildren(const Options &options) {
    DCSDeviceController<MKS946>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946M]);
    addVariableUpdate(m, 1000, &DCSMKS946Controller::getMeasurements, this, options);
    m.setHistorizing();
    auto &r = addVariable("relay", &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_RELAY]);
    addVariableUpdate(r, 1000, &DCSMKS946Controller::getRelay, this, options);
    addControllerMethod("setrelay", "Sets relay",
                        {{"Relay number", "1-3", &UA_TYPES[UA_TYPES_UINT32]},
                         {"Enabled", "OFF/ON", &UA_TYPES[UA_TYPES_UINT32]},
                         {"Setpoint", "Current units", &UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Hysteresis", "Current units", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSMKS946Controller::setRelay, this);
    auto &c = addVariable("configuration",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_MKS946C]);
    addVariableUpdate(c, 600000, &DCSMKS946Controller::getConfiguration, this, options);
    c.setHistorizing();
    auto &p = addVariable("PID", &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_PID]);
    addVariableUpdate(p, 600000, &DCSMKS946Controller::getPID, this, options);
    p.setHistorizing();
    addVariable("sensorType", &UA_TYPES[UA_TYPES_STRING], true);
    addControllerMethod("configurePID", "sets parameters of active PID recipe",
                        {
                            {"MFC channel", "", &UA_TYPES[UA_TYPES_STRING]},
                            {"Pressure channel", "", &UA_TYPES[UA_TYPES_STRING]},
                            {"Pressure setpoint", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Kp", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Time constant", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Derivative time constant", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Ceiling", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Base", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Preset", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Start", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"End", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"CtrlStart", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                            {"Direction", "", &UA_TYPES[UA_TYPES_STRING]},
                            {"Band", "", &UA_TYPES[UA_TYPES_UINT32]},
                            {"Gain", "", &UA_TYPES[UA_TYPES_UINT32]},
                        },
                        {}, &DCSMKS946Controller::configurePID, this);
    addControllerMethod("configureMFC", "sets parameters of mfc",
                        {{"Mode", "", &UA_TYPES[UA_TYPES_STRING]},
                         {"Setpoint", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Nominal range", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Scale factor", "", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSMKS946Controller::configureFlow, this);
    addControllerMethod("configureManometer", "sets parameters of manometer",
                        {{"Type", "", &UA_TYPES[UA_TYPES_STRING]},
                         {"Nominal range", "", &UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Voltage range", "", &UA_TYPES[UA_TYPES_STRING]}},
                        {}, &DCSMKS946Controller::configurePressure, this);
    addControllerMethod("setPIDState", "sets PID control mode ON/OFF",
                        {{"State", "", &UA_TYPES[UA_TYPES_BOOLEAN]}}, {},
                        &DCSMKS946Controller::setPIDState, this);
    auto &ps = addVariable("PIDState", &UA_TYPES[UA_TYPES_BOOLEAN]);
    addVariableUpdate(ps, 1000, &DCSMKS946Controller::getPIDState, this, options);
    ps.setHistorizing();
    addControllerMethod("zeroMFC", "zero MFC Channel", {}, {},
                        [this](const UA_Variant *in, UA_Variant *out) {
                            device.setUserCalibrationEnabled(true);
                            device.zeroMFC(flowCH);
                            device.setUserCalibrationEnabled(false);
                        });
    addInterlock(options);
}

UA_MKS946m DCSMKS946Controller::getMeasurements() {
    UA_MKS946m mks;
    UA_MKS946m_init(&mks);
    auto flow = device.getFlow(flowCH);
    if(flow.at(0) == '<' || flow.at(0) == '>') {
        flow = flow.substr(1, flow.size() - 1);
    }
    mks.flow = std::stod(flow);
    mks.pressure = std::stod(device.getPressure(pressureCH));
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

UA_MKS946c DCSMKS946Controller::getConfiguration() {
    UA_MKS946c mks;
    UA_MKS946c_init(&mks);
    mks.flowMode = UA_STRING_ALLOC(device.getFlowMode(flowCH).c_str());
    mks.flowNominalRange = std::stod(device.getFlowNominalRange(flowCH));
    mks.flowScaleFactor = std::stod(device.getFlowScaleFactor(flowCH));
    mks.flowSetPoint = std::stod(device.getFlowSetPoint(flowCH));
    mks.manometerNominalRange = std::stod(device.getCMNominalRange(pressureCH));
    mks.manometerType = UA_STRING_ALLOC(device.getCMType(pressureCH).c_str());
    mks.manometerVoltageRange =
        UA_STRING_ALLOC(device.getCMVoltageRange(pressureCH).c_str());
    return mks;
}

UA_PID DCSMKS946Controller::getPID() {
    UA_PID mks;
    UA_PID_init(&mks);
    mks.direction = UA_STRING_ALLOC(device.getPIDDirection().c_str());
    mks.band = std::stoi(device.getPIDBand());
    mks.base = std::stod(device.getPIDBase());
    mks.ceiling = std::stod(device.getPIDCeiling());
    mks.ctrlStart = std::stod(device.getPIDCtrlStart());
    mks.derivativeTimeConstant = std::stod(device.getPIDDerivativeTimeConstant());

    mks.end = std::stod(device.getPIDEnd());
    mks.flowChannel = UA_STRING_ALLOC(device.getPIDMFCChannel().c_str());
    mks.gain = std::stoi(device.getPIDGain());
    mks.kp = std::stod(device.getPIDKp());
    mks.preset = std::stod(device.getPIDPreset());
    mks.pressureChannel = UA_STRING_ALLOC(device.getPIDPressureChannel().c_str());
    mks.pressureSetPoint = std::stod(device.getPIDPressureSetPoint());
    mks.recipeNr = std::stoi(device.getActivePIDRecipe());
    mks.start = std::stod(device.getPIDStart());
    mks.timeConstant = std::stod(device.getPIDTimeConstant());
    mks.units = UA_STRING_ALLOC(device.getUnits().c_str());
    return mks;
}

void DCSMKS946Controller::postConnect() {
    size_t size = 4;
    auto *array =
        static_cast<UA_String *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_STRING]));
    std::istringstream ss(device.getModuleType());
    for(size_t i = 0; i < size; ++i) {
        std::string val;
        std::getline(ss, val, ',');
        array[i] = UA_STRING_ALLOC(val.c_str());
    }
    variables.at("sensorType")->setValue(array, size);
    variables.at("PID")->setValue(getPID());
    variables.at("configuration")->setValue(getConfiguration());
}

void DCSMKS946Controller::configurePID(const UA_Variant *input, UA_Variant *output) {
    auto mfc = *static_cast<UA_String *>(input[0].data);
    auto prc = *static_cast<UA_String *>(input[1].data);
    auto setpoint = *static_cast<UA_Double *>(input[2].data);
    auto kp = *static_cast<UA_Double *>(input[3].data);
    auto timeConstant = *static_cast<UA_Double *>(input[4].data);
    auto derivativeTimeConstant = *static_cast<UA_Double *>(input[5].data);
    auto ceiling = *static_cast<UA_Double *>(input[6].data);
    auto base = *static_cast<UA_Double *>(input[7].data);
    auto preset = *static_cast<UA_Double *>(input[8].data);
    auto start = *static_cast<UA_Double *>(input[9].data);
    auto end = *static_cast<UA_Double *>(input[10].data);
    auto ctrlstart = *static_cast<UA_Double *>(input[11].data);
    auto direction = *static_cast<UA_String *>(input[12].data);
    auto band = *static_cast<UA_UInt32 *>(input[13].data);
    auto gain = *static_cast<UA_UInt32 *>(input[14].data);
    if(!getPIDState()) {
        device.setPIDMFCChannel(
            MKS946codes::PIDFlowChannelFromString.at(DCSUtils::UaToStd(mfc)));
        device.setPIDPressureChannel(
            MKS946codes::PIDPressureChannelFromString.at(DCSUtils::UaToStd(prc)));
        device.setPIDCeiling(ceiling);
        device.setPIDBase(base);
        device.setPIDPreset(preset);
        device.setPIDStart(start);
        device.setPIDEnd(end);
        device.setPIDCtrlStart(ctrlstart);
        device.setPIDDirection(
            MKS946codes::PIDDirectionFromString.at(DCSUtils::UaToStd(direction)));
        device.setPIDGain(gain);
        device.setPIDBand(band);
    }
    device.setPIDPressureSetPoint(setpoint);
    device.setPIDKp(kp);
    device.setPIDTimeConstant(timeConstant);
    device.setPIDDerivativeTimeConstant(derivativeTimeConstant);
    variables.at("PID")->setValue(getPID());
}

void DCSMKS946Controller::configureFlow(const UA_Variant *input, UA_Variant *output) {
    auto mode = *static_cast<UA_String *>(input[0].data);
    auto setpoint = *static_cast<UA_Double *>(input[1].data);
    auto nominalRange = *static_cast<UA_Double *>(input[2].data);
    auto scaleFactor = *static_cast<UA_Double *>(input[3].data);
    device.setFlowMode(flowCH,
                       MKS946codes::flowModeFromString.at(DCSUtils::UaToStd(mode)));
    device.setFlowSetPoint(flowCH, setpoint);
    device.setFlowNominalRange(flowCH, nominalRange);
    device.setFlowScaleFactor(flowCH, scaleFactor);

    variables.at("configuration")->setValue(getConfiguration());
}

void DCSMKS946Controller::configurePressure(const UA_Variant *input, UA_Variant *output) {
    auto type = *static_cast<UA_String *>(input[0].data);
    auto nominalRange = *static_cast<UA_Double *>(input[1].data);
    auto voltageRange = *static_cast<UA_String *>(input[2].data);
    device.setCMType(pressureCH,
                     MKS946codes::CMTypeFromString.at(DCSUtils::UaToStd(type)));
    device.setCMNominalRange(pressureCH, nominalRange);
    device.setCMVoltageRange(pressureCH, MKS946codes::CMVoltageRangeFromString.at(
                                             DCSUtils::UaToStd(voltageRange)));

    variables.at("configuration")->setValue(getConfiguration());
}

void DCSMKS946Controller::setPIDState(const UA_Variant *input, UA_Variant *output) {
    auto state = *static_cast<UA_Boolean *>(input[0].data);
    device.setPIDControl(state);
    variables.at("configuration")->setValue(getConfiguration());
}

UA_Boolean DCSMKS946Controller::getPIDState() {
    auto response = device.getPIDControl();
    if(response.find("ON") != response.npos) {
        return true;
    } else if(response == "OFF") {
        return false;
    } else {
        throw std::runtime_error("Received incorrect pid state: " + response);
    }
}

void DCSMKS946Controller::addInterlock(const Options &options) {
    auto &interlockState = addVariable("interlockState", &UA_TYPES[UA_TYPES_BOOLEAN]);
    auto &interlockEnabled = addVariable("interlockEnabled", &UA_TYPES[UA_TYPES_BOOLEAN]);
    auto &interlockLimit = addVariable("interlockLimit", &UA_TYPES[UA_TYPES_DOUBLE]);
    addVariableUpdate(interlockState, 1000, &DCSMKS946Controller::interlockAction, this,
                      options);
    interlockEnabled.setValue(options.value("interlockEnabled", false));
    interlockLimit.setValue(options.value("interlockLimit", std::nan("")));
    addControllerMethod("configureInterlock", "Configures software interlock",
                        {{"Enabled", "OFF/ON", &UA_TYPES[UA_TYPES_BOOLEAN]},
                         {"Limit", "Upper bound", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSMKS946Controller::configureInterlock, this);
}

bool DCSMKS946Controller::interlockAction() {
    if(!variables.at("interlockEnabled")->getValue<bool>()) {
        return false;
    }
    auto setValue = variables.at("interlockLimit")->getValue<double>();
    if(!std::isnan(setValue) && (setValue < std::stod(device.getPressure(pressureCH)))) {
        device.setFlowMode(flowCH, MKS946codes::FlowMode::CLOSE);
        variables.at("configuration")->setValue(getConfiguration());
        return true;
    }
    return false;
}

void DCSMKS946Controller::configureInterlock(const UA_Variant *input,
                                             UA_Variant *output) {
    auto enabled = *static_cast<UA_Boolean *>(input[0].data);
    auto limit = *static_cast<UA_Double *>(input[1].data);
    variables.at("interlockEnabled")->setValue(enabled);
    variables.at("interlockLimit")->setValue(limit);
}
