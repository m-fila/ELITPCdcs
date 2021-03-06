#include "DCSN1471Controller.h"
#include <sstream>
void DCSN1471Controller::addChildren(const Options &options) {
    DCSDeviceController<N1471>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_DT1415M]);
    addVariableUpdate(m, 1000, &DCSN1471Controller::getMeasurements, this, options);
    auto &c = addVariable("configuration",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_DT1415C]);
    addVariableUpdate(c, 1000, &DCSN1471Controller::getConfiguration, this, options);

    addControllerMethod("setchannel", "Sets channel ON/OFF",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"State", "ON/OFF", &UA_TYPES[UA_TYPES_BOOLEAN]}},
                        {}, &DCSN1471Controller::setChannel, this);
    addControllerMethod("setvoltage", "Sets voltage",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Voltage", "Voltage in V", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSN1471Controller::setVoltage, this);
    addControllerMethod(
        "setvoltagemax", "Sets maximum voltage",
        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
         {"Maximum voltage", "Maximum voltage in V", &UA_TYPES[UA_TYPES_DOUBLE]}},
        {}, &DCSN1471Controller::setVoltageMax, this);
    addControllerMethod("setcurrent", "Sets current",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Current", "Current in A", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSN1471Controller::setCurrent, this);
    addControllerMethod("setrampup", "Sets ramp up",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Ramp up", "Ramp up in V/s", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSN1471Controller::setRampUp, this);
    addControllerMethod("setrampdown", "Sets ramp down",
                        {{"Channel", "Channels number", &UA_TYPES[UA_TYPES_INT16]},
                         {"Ramp down", "Ramp down in V/s", &UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSN1471Controller::setRampDown, this);
    addControllerMethod("clearalarm", "Clears alarm signal", {}, {},
                        &DCSN1471Controller::clearAlarm, this);
}

UA_DT1415m DCSN1471Controller::getMeasurements() {
    UA_DT1415m dtm;
    UA_DT1415m_init(&dtm);
    size_t size = 4;
    dtm.voltageSize = size;
    dtm.currentSize = size;
    dtm.voltage =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtm.current =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    // voltages
    std::string val;
    std::string response = device.getVoltage(N1471::CHANNEL::ALL);
    float total = 0;
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtm.voltage[i] = std::stod(val);
            total += dtm.voltage[i];
        }
    }
    dtm.totalVoltage = total;
    // currents
    response = device.getCurrent(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtm.current[i] = std::stod(val);
        }
    }
    return dtm;
}

UA_DT1415c DCSN1471Controller::getConfiguration() {
    UA_DT1415c dtc;
    UA_DT1415c_init(&dtc);
    size_t size = 4;
    dtc.voltageSetSize = size;
    dtc.currentSetSize = size;
    dtc.statusSize = size;
    dtc.voltageMaxSize = size;
    dtc.rupSize = size;
    dtc.rdownSize = size;
    dtc.polaritySize = size;
    dtc.voltageSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.currentSet =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.status = static_cast<UA_UInt32 *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    dtc.voltageMax =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rdown = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rup = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.polarity =
        static_cast<UA_String *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_STRING]));
    dtc.isRemote = device.isRemote();
    // device status
    std::string val;
    std::string response = device.getStatus(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; ++i) {
            std::getline(iss, val, ';');
            dtc.status[i] = std::stoi(val);
        }
    }
    // voltage set
    float total = 0;
    response = device.getVoltageSet(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.voltageSet[i] = std::stod(val);
            total += dtc.voltageSet[i];
        }
    }

    response = device.getCurrentSet(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.currentSet[i] = std::stod(val);
        }
    }
    dtc.totalVoltageSet = total;

    response = device.getRampUp(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.rup[i] = std::stod(val);
        }
    }

    response = device.getRampDown(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.rdown[i] = std::stod(val);
        }
    }

    response = device.getVoltageMax(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.voltageMax[i] = std::stod(val);
        }
    }

    response = device.getPolarity(N1471::CHANNEL::ALL);
    {
        std::istringstream iss(response);
        for(size_t i = 0; i < size; i++) {
            std::getline(iss, val, ';');
            dtc.polarity[i] = UA_STRING_ALLOC(val.c_str());
        }
    }
    return dtc;
}

void DCSN1471Controller::setChannel(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Boolean state = *(UA_Boolean *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    if(state) {
        device.setON(CH);
    } else {
        device.setOFF(CH);
    }
}
void DCSN1471Controller::setVoltage(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double voltage = *(UA_Double *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    device.setVoltageSet(CH, voltage);
}
void DCSN1471Controller::setCurrent(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double current = *(UA_Double *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    device.setCurrentSet(CH, current);
}
void DCSN1471Controller::setVoltageMax(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double voltage = *(UA_Double *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    device.setVoltageMax(CH, voltage);
}
void DCSN1471Controller::setRampUp(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double ramp = *(UA_Double *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    device.setRampUp(CH, ramp);
}
void DCSN1471Controller::setRampDown(const UA_Variant *input, UA_Variant *output) {
    UA_Int16 channel = *(UA_Int16 *)input[0].data;
    UA_Double ramp = *(UA_Double *)input[1].data;
    N1471::CHANNEL CH = static_cast<N1471::CHANNEL>(channel);
    device.setRampDown(CH, ramp);
}
