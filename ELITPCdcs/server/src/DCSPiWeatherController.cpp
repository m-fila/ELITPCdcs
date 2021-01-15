#include "DCSPiWeatherController.h"
#include <sstream>

void DCSPiWeatherController::addChildren(const Options &options) {
    DCSDeviceController<PiWeather>::addChildren(options);
    auto &m = addVariable("measurements",
                          &UA_TYPES_ELITPCNODESET[UA_TYPES_ELITPCNODESET_PIWEATHERM]);
    addVariableUpdate(
        m, 500, [this]() { return getMeasurements(); }, options);
    m.setHistorizing("default");
}

UA_PiWeatherm DCSPiWeatherController::getMeasurements() {
    int size = 3;
    UA_PiWeatherm m;
    UA_PiWeatherm_init(&m);
    std::string response = device.getMeasurements();
    m.tempSize = size;
    m.temp = static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));

    std::istringstream iss(response);
    std::string value;
    for(int i = 0; i < size; i++) {
        std::getline(iss, value, ';');
        m.temp[i] = std::stod(value);
    }
    std::getline(iss, value, ';');
    m.temperature = std::stod(value);
    std::getline(iss, value, ';');
    m.pressure = std::stod(value);
    return m;
}
