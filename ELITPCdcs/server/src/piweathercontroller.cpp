#include "piweathercontroller.h"
#include <sstream>

PiWeatherController::PiWeatherController(std::string name):
    OpcTemplateController<UA_PiWeatherm,UA_PiWeatherc,PiWeather>(name,
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_PIWEATHERM],
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_PIWEATHERC]){
}


void PiWeatherController::init(UA_Server *server){
    addObject(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    spawn_thread();
}

UA_PiWeatherm PiWeatherController::getMeasurements(){
    int size=3;
    UA_PiWeatherm tpg;
    UA_PiWeatherm_init(&tpg);
    tpg.tempSize=size;
    tpg.temp=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    std::string response=device.getMeasurements();
    std::istringstream iss(response);
    std::string value;
    for(int i=0; i<size; i++){
        std::getline(iss, value, ';');
        tpg.temp[i]=std::stod(value);
    }
    std::getline(iss, value, ';');
    tpg.temperature=std::stod(value);
    std::getline(iss, value, ';');
    tpg.pressure=std::stod(value);
    return tpg;
}




UA_PiWeatherc PiWeatherController::getSettings(){
    UA_PiWeatherc tpg;
    UA_PiWeatherc_init(&tpg);
    return tpg;
}



