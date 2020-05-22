#include "DCSTPG362Controller.h"
#include <sstream>
DCSTPG362Controller::DCSTPG362Controller(UA_Server *server, std::string name)
    : DCSDeviceController(server, name) {
  auto &m = addVariable("measurements",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362M]);
  addVariableUpdate(m, 1000, &DCSTPG362Controller::getMeasurements, this);
  auto &c = addVariable("configuration",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362C]);
  addVariableUpdate(c, 1000, &DCSTPG362Controller::getConfiguration, this);
}

UA_TPG362m DCSTPG362Controller::getMeasurements(){
    int size=2;
    UA_TPG362m tpg;
    UA_TPG362m_init(&tpg);
    tpg.vacuumSize=size;
    tpg.vacuum=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    tpg.statusSize=size;
    tpg.status=static_cast<UA_UInt32*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    std::string response=device.getGaugesData(TPG362::CH::ALL);
    std::istringstream iss(response);
    for(int i=0; i<size; i++){
        std::string value;
        std::string status;
        std::getline(iss, status, ',');
        std::getline(iss, value, ',');
        tpg.vacuum[i]=std::stod(value);
        tpg.status[i]=std::stoi(status);
    }
    return tpg;
}

UA_TPG362c DCSTPG362Controller::getConfiguration(){
    int size=2;
    UA_TPG362c tpg;
    UA_TPG362c_init(&tpg);

    tpg.pressureSize=size;
    tpg.pressure=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    return tpg;
}
