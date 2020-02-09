#include "tpg362controller.h"
#include <sstream>

TPG362Controller::TPG362Controller(std::string name): 
    OpcTemplateController<UA_TPG362m,UA_TPG362c,TPG362>(name,
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362M],
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_TPG362C]){

}


void TPG362Controller::init(UA_Server *server){
    addObject(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    spawn_thread();
}

UA_TPG362m TPG362Controller::getMeasurements(){
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




UA_TPG362c TPG362Controller::getSettings(){
    int size=2;
    UA_TPG362c tpg;
    UA_TPG362c_init(&tpg);

    tpg.pressureSize=size;
    tpg.pressure=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
   /* for(int i=0; i!=size;++i){
        device.setActiveChannel(i+1);
        response = device.getOutputSel();
        tpg.ch[i]=(response=="1") ? true : false;
        tpg.current[i]=std::stod(device.getCurrent());
        tpg.voltage[i]=std::stod(device.getVoltage());
    }
    */
    return tpg;
}



