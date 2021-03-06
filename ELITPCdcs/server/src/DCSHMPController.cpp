#include "DCSHMPController.h"
void DCSHMPController::addChildren(const Options &options) {
    DCSDeviceController<HMP2020>::addChildren(options);
    auto &m =
        addVariable("measurements", &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_HMPM]);
    addVariableUpdate(m, 1000, &DCSHMPController::getMeasurements, this, options);
    m.setHistorizing();
    auto &c = addVariable("configuration",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_HMPC]);
    addVariableUpdate(c, 1000, &DCSHMPController::getConfiguration, this, options);
    addControllerMethod("setchannel", "Sets channel ON/OFF",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"State", "ON/OFF", &UA_TYPES[UA_TYPES_BOOLEAN]}},
                        {}, &DCSHMPController::setChannel, this);
    addControllerMethod("setoutput", "Sets output ON/OFF",
                        {{"State", "ON/OFF", &UA_TYPES[UA_TYPES_BOOLEAN]}}, {},
                        &DCSHMPController::setOutput, this);
    addControllerMethod("setvoltage", "Sets voltage",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Voltage", "Voltage in V", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSHMPController::setVoltage, this);
    addControllerMethod("setcurrent", "Sets current",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Current", "Current in A", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSHMPController::setCurrent, this);
    addProfiles(c, options);
}

UA_HMPm DCSHMPController::getMeasurements() {
    std::string response;
    UA_HMPm hmp;
    UA_HMPm_init(&hmp);
    response = device.getOutputGen();
    hmp.output = (response == "1") ? true : false;
    hmp.chSize = size;
    hmp.currentSize = size;
    hmp.voltageSize = size;
    hmp.ch = static_cast<UA_Boolean *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_BOOLEAN]));
    hmp.current =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    hmp.voltage =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    for(int i = 0; i != size; ++i) {
        device.setActiveChannel(i + 1);
        response = device.getOutputSel();
        hmp.ch[i] = (response == "1") ? true : false;
        hmp.current[i] = std::stod(device.getCurrent());
        hmp.voltage[i] = std::stod(device.getVoltage());
    }
    return hmp;
}

UA_HMPc DCSHMPController::getConfiguration() {
    UA_HMPc hmp;
    UA_HMPc_init(&hmp);
    hmp.currentSetSize = size;
    hmp.voltageSetSize = size;
    hmp.currentSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    hmp.voltageSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    for(int i = 0; i != size; i++) {
        device.setActiveChannel(i + 1);
        hmp.currentSet[i] = std::stod(device.getCurrentSet());
        hmp.voltageSet[i] = std::stod(device.getVoltageSet());
    }
    return hmp;
}

void DCSHMPController::setChannel(const UA_Variant *input, UA_Variant *) {
    int channel = *static_cast<UA_Int16 *>(input[0].data);
    bool state = *static_cast<UA_Boolean *>(input[1].data);
    device.setOutputSel(channel, state);
}

void DCSHMPController::setOutput(const UA_Variant *input, UA_Variant *) {
    bool state = *static_cast<UA_Boolean *>(input[0].data);
    device.setOutputGen(state);
}

void DCSHMPController::setVoltage(const UA_Variant *input, UA_Variant *) {
    int channel = *static_cast<UA_Int16 *>(input[0].data);
    double v = *static_cast<UA_Double *>(input[1].data);
    device.setVoltage(channel, v);
}

void DCSHMPController::setCurrent(const UA_Variant *input, UA_Variant *) {
    int channel = *static_cast<UA_Int16 *>(input[0].data);
    double i = *static_cast<UA_Double *>(input[1].data);
    device.setCurrent(channel, i);
}

void DCSHMPController::parseProfile(const Options &options) {
    if(options.contains("CurrentSet")) {
        auto o = options.at("CurrentSet");
        for(size_t i = 0; i < o.size(); ++i) {
            device.setCurrent(i + 1, o.at(i).get<double>());
        }
    }

    if(options.contains("VoltageSet")) {
        auto o = options.at("VoltageSet");
        for(size_t i = 0; i < o.size(); ++i) {
            device.setVoltage(i + 1, o.at(i).get<double>());
        }
    }
}
