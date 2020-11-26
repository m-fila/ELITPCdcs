#include "DCSTPG362Controller.h"
#include <sstream>
void DCSTPG362Controller::addChildren(const Options &options) {
    addConnection();
    auto &m =
        addVariable("measurements", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362M]);
    addVariableUpdate(m, 1000, &DCSTPG362Controller::getMeasurements, this);
    m.setHistorizing();
    auto &c =
        addVariable("configuration", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362C]);
    addVariableUpdate(c, 1000, &DCSTPG362Controller::getConfiguration, this);
    auto &r = addVariable("relay", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_RELAY]);
    addVariableUpdate(r, 1000, &DCSTPG362Controller::getRelay, this);
    addControllerMethod("setrelay", "Sets relay",
                        {{"Relay number", "1-4", UA_TYPES[UA_TYPES_UINT32]},
                         {"Enabled", "OFF/ON/CH1/CH2", UA_TYPES[UA_TYPES_UINT32]},
                         {"Setpoint", "Current units", UA_TYPES[UA_TYPES_DOUBLE]},
                         {"Hysteresis", "Current units", UA_TYPES[UA_TYPES_DOUBLE]}},
                        {}, &DCSTPG362Controller::setRelay, this);
}

UA_TPG362m DCSTPG362Controller::getMeasurements() {

    int size = 2;
    UA_TPG362m tpg;
    UA_TPG362m_init(&tpg);
    tpg.vacuumSize = size;
    tpg.vacuum = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    tpg.statusSize = size;
    tpg.status = static_cast<UA_UInt32 *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    size_t tries = 10;
    while(tries) {
        try {
            std::string response = device.getGaugesData(TPG362::CH::ALL);
            std::istringstream iss(response);
            for(int i = 0; i < size; i++) {
                std::string value;
                std::string status;
                std::getline(iss, status, ',');
                std::getline(iss, value, ',');
                tpg.vacuum[i] = std::stod(value);
                tpg.status[i] = std::stoi(status);
            }
            break;
        } catch(const std::exception &e) {
            --tries;
            if(tries == 0) {
                throw std::runtime_error(e.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    return tpg;
}

UA_TPG362c DCSTPG362Controller::getConfiguration() {
    int size = 2;
    UA_TPG362c tpg;
    UA_TPG362c_init(&tpg);

    tpg.pressureSize = size;
    tpg.pressure =
        static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    return tpg;
}

UA_Relay DCSTPG362Controller::getRelay() {
    size_t size = 4;
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
    size_t tries = 10;
    while(tries) {
        try {
            auto statusResponse = device.getSwitchingFunctionStatus();
            std::istringstream statusStream(statusResponse);
            for(size_t i = 0; i < size; ++i) {
                relay.direction[i] = false;
                std::string val;
                std::getline(statusStream, val, ',');
                relay.status[i] = std::stoi(val);
                auto function = device.getSwitchingFunction(
                    static_cast<TPG362::SWITCHING_FUNCTION>(i + 1));
                std::istringstream functionStream(function);
                std::getline(functionStream, val, ',');
                relay.enabled[i] = std::stoi(val);
                std::getline(functionStream, val, ',');
                relay.setpoint[i] = std::stod(val);
                std::getline(functionStream, val, ',');
                relay.hysteresis[i] = std::stod(val);
            }
            break;
        } catch(const std::exception &e) {
            --tries;
            if(tries == 0) {
                throw std::runtime_error(e.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    return relay;
}

void DCSTPG362Controller::setRelay(const UA_Variant *input, UA_Variant *output) {
    auto function = *static_cast<UA_UInt32 *>(input[0].data);
    auto assignment = *static_cast<UA_UInt32 *>(input[1].data);
    auto setpoint = *static_cast<UA_Double *>(input[2].data);
    auto hysteresis = *static_cast<UA_Double *>(input[3].data);
    device.setSwitchingFunction(static_cast<TPG362::SWITCHING_FUNCTION>(function),
                                static_cast<TPG362::SWITCHING_STATUS>(assignment),
                                setpoint, hysteresis);
}
