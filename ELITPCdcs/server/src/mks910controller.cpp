#include "mks910controller.h"
#include <sstream>

MKS910Controller::MKS910Controller(std::string name): 
    OpcTemplateController<UA_MKS910m,UA_MKS910c,MKS910>(name,
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910M],
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910C]){

}


void MKS910Controller::init(UA_Server *server){
    addObject(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    addSetUnitsMethod(server);
    spawn_thread();
}

UA_MKS910m MKS910Controller::getMeasurements(){
    UA_MKS910m mks;
    UA_MKS910m_init(&mks);
    mks.vacuum=std::stod(device.getPCombinedLong());
    mks.temperature=std::stod(device.getTemp());
    mks.status=static_cast<int>(MKS910codes::statusFromString.at(device.getStatus()));
    mks.units=static_cast<int>(MKS910codes::unitsFromString.at(device.getUnits()));
    return mks;
}


UA_MKS910c MKS910Controller::getSettings(){
    UA_MKS910c mks;
    UA_MKS910c_init(&mks);
    return mks;
}

UA_StatusCode MKS910Controller::setUnitsCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    MKS910Controller* monitor=static_cast<MKS910Controller*>(methodContext);
    if(monitor->isConnected()){
        UA_Int32 units = *(UA_Int32*)input->data;
    //    std::lock_guard<std::mutex> lock(Monitor->deviceMutex);
        DeviceCommand<MKS910> command=std::bind(&MKS910::setUnits, _1,static_cast<MKS910codes::Units>(units));
        monitor->buffer.push(command);
       // monitor->device.setOutputGen(state);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetOutput not send");
    }
    return UA_STATUSCODE_GOOD;
}

void MKS910Controller::addSetUnitsMethod(UA_Server *server) {
    addMethod(server,"setunits",setUnitsCallback,"Sets pressure units",{{UA_TYPES[UA_TYPES_INT32],"Unit","PASCAL/BAR/TORR"}},{});
}

