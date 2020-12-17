#include "DCSDT1470ETController.h"
void DCSDT1470ETController::addChildren(const Options &options) {
    DCSDeviceController<DT1470ET>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_DT1415M]);
    addVariableUpdate(m, 1000, &DCSDT1470ETController::getMeasurements, this);
    auto &c = addVariable("configuration",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_DT1415C]);
    addVariableUpdate(c, 1000, &DCSDT1470ETController::getConfiguration, this);

    addControllerMethod("setchannel", "Sets channel ON/OFF",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"State", "ON/OFF", &UA_TYPES[UA_TYPES_BOOLEAN]}},
                        {}, &DCSDT1470ETController::setChannel, this);
    addControllerMethod("setvoltage", "Sets voltage",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Voltage", "Voltage in V", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSDT1470ETController::setVoltage, this);
    addControllerMethod(
        "setvoltagemax", "Sets maximum voltage",
        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
         {"Maximum voltage", "Maximum voltage in V", &UA_TYPES[UA_TYPES_DOUBLE]}},
        {}, &DCSDT1470ETController::setVoltageMax, this);
    addControllerMethod("setcurrent", "Sets current",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Current", "Current in A", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSDT1470ETController::setCurrent, this);
    addControllerMethod("setrampup", "Sets ramp up",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Ramp up", "Ramp up in V/s", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSDT1470ETController::setRampUp, this);
    addControllerMethod("setrampdown", "Sets ramp down",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Ramp down", "Ramp down in V/s", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSDT1470ETController::setRampDown, this);
}

UA_DT1415m DCSDT1470ETController::getMeasurements() {
    UA_DT1415m dtm;
    UA_DT1415m_init(&dtm);
    int size = 4;
    dtm.voltageSize = size;
    dtm.currentSize = size;
    dtm.voltage =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtm.current =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    // voltages
    std::string response = device.getVoltage(DT1470ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    float total = 0;
    for(i = 0; i < size; ++i) {
        std::getline(iss, val, ';');
        dtm.voltage[i] = std::stod(val.c_str());
        total += dtm.voltage[i];
    }
    dtm.totalVoltage = total;
    // currents
    response = device.getCurrent(DT1470ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i = 0; i < size; i++) {
        std::getline(iss2, val, ';');
        dtm.current[i] = std::stod(val.c_str());
    }

    return dtm;
}

UA_DT1415c DCSDT1470ETController::getConfiguration() {
    UA_DT1415c dtc;
    UA_DT1415c_init(&dtc);
    int size = 4;
    dtc.voltageSetSize = size;
    dtc.currentSetSize = size;
    dtc.statusSize = size;
    dtc.voltageMaxSize = size;
    dtc.rupSize = size;
    dtc.rdownSize = size;
    dtc.voltageSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.currentSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.status = static_cast<UA_UInt32 *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    dtc.voltageMax =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rdown = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rup = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.isRemote = device.isRemote();
    // device status
    std::string response = device.getStatus(DT1470ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    for(i = 0; i < size; ++i) {
        std::getline(iss, val, ';');
        dtc.status[i] = std::stoi(val.c_str());
    }
    // voltage set
    float total = 0;
    response = device.getVoltageSet(DT1470ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i = 0; i < size; i++) {
        std::getline(iss2, val, ';');
        dtc.voltageSet[i] = std::stod(val.c_str());
        total += dtc.voltageSet[i];
    }

    response = device.getCurrentSet(DT1470ET::CHANNEL::ALL);
    std::istringstream iss6(response);
    for(i = 0; i < size; i++) {
        std::getline(iss6, val, ';');
        dtc.currentSet[i] = std::stod(val.c_str());
    }

    dtc.totalVoltageSet = total;

    response = device.getRampUp(DT1470ET::CHANNEL::ALL);
    std::istringstream iss3(response);
    for(i = 0; i < size; ++i) {
        std::getline(iss3, val, ';');
        dtc.rup[i] = std::stoi(val.c_str());
    }

    response = device.getRampDown(DT1470ET::CHANNEL::ALL);
    std::istringstream iss4(response);
    for(i = 0; i < size; ++i) {
        std::getline(iss4, val, ';');
        dtc.rdown[i] = std::stoi(val.c_str());
    }
    response = device.getVoltageMax(DT1470ET::CHANNEL::ALL);
    std::istringstream iss5(response);
    for(i = 0; i < size; ++i) {
        std::getline(iss5, val, ';');
        dtc.voltageMax[i] = std::stoi(val.c_str());
    }

    return dtc;
}

void DCSDT1470ETController::setChannel(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Boolean state = *(UA_Boolean *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    if(state) {
        device.setON(CH);
    } else {
        device.setOFF(CH);
    }
}
void DCSDT1470ETController::setVoltage(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double voltage = *(UA_Double *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    device.setVoltageSet(CH, voltage);
}
void DCSDT1470ETController::setCurrent(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double current = *(UA_Double *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    device.setCurrentSet(CH, current);
}
void DCSDT1470ETController::setVoltageMax(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double voltage = *(UA_Double *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    device.setVoltageMax(CH, voltage);
}
void DCSDT1470ETController::setRampUp(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double ramp = *(UA_Double *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    device.setRampUp(CH, ramp);
}
void DCSDT1470ETController::setRampDown(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double ramp = *(UA_Double *)input[1].data;
    DT1470ET::CHANNEL CH = static_cast<DT1470ET::CHANNEL>(channel);
    device.setRampDown(CH, ramp);
}
