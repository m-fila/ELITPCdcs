#include "include/opc/dtcontroller.h"
#include <sstream>
DTController::DTController(std::string name): opc_template_controller<DTMeasurements,DTConfiguration,DT1415ET>(name){
    VariableTypeM=customTypeM.Type;
    VariableTypeC=customTypeC.Type;
}

DTController::~DTController(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
}

DTMeasurements DTController::getMeasurements(){
    DTMeasurements dtm;
    //voltages
    std::string response = device->getVoltage(DT1415ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    float total = 0;
    for(i=0; i<8; i++)
    {
        std::getline(iss, val, ';');
        dtm.voltage[i] = std::stof(val.c_str());
        total += dtm.voltage[i];
        //std::cout << voltageMeasurements.voltage[i] << std::endl;
    }
    dtm.totalVoltage = total;
    //currents
    response = device->getCurrent(DT1415ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<8; i++)
    {
        std::getline(iss2, val, ';');
        dtm.current[i] = std::stof(val.c_str());
        //std::cout << measurements.current[i] << std::endl;
    }

    return dtm;
}

DTConfiguration DTController::getSettings(){
    DTConfiguration dtc;
    dtc.isRemote = device->isRemote();
    //device status
    std::string response = device->getStatus(DT1415ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    for(i=0; i<8; i++)
    {
        std::getline(iss, val, ';');
        dtc.status[i] = std::stoi(val.c_str());
        //std::cout << val << std::endl;
    }
    //voltage set
    float total = 0;
    response = device->getVoltageSet(DT1415ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<8; i++)
    {
        std::getline(iss2, val, ';');
        //std::cout << val << std::endl;
        dtc.voltageSet[i] = std::stof(val.c_str());
        total += dtc.voltageSet[i];
    }
    dtc.totalVoltageSet= total;
    return dtc;
}
