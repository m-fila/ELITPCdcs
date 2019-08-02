#ifndef OPC_TEMPLATE_CONTROLLER_H
#define OPC_TEMPLATE_CONTROLLER_H
#include <open62541/server.h>
#include "opc_monitor.h"


#include "../ConnectionParameters.h"
#include "../utl/TCPConnector.h"
#include "devicebuffer.h"
#include <functional>
template <class D>
using DeviceCommand = std::function<void(D&)>;

using std::placeholders::_1;

template <class M,class C,class D>
class OpcTemplateController :public OpcMonitor
{

public:
    OpcTemplateController(std::string name) : OpcMonitor(name){
        disconnectDevice();
    }

//    virtual ~OpcTemplateController(){}

private:
    virtual M getMeasurements()=0;
    virtual C getSettings()=0;
    void disconnectDevice() override;
    void connectDevice(TCPConnectionParameters *cp) override;
    void updateMeasurementsVariable(UA_Server *server) override;
    void updateConfigurationVariable(UA_Server *server) override;
    void updateStatusVariable(UA_Server *server) override;
    void update_configuration();
    void update_measurements();
    void update_status();

    std::mutex mMutex;
    std::mutex cMutex;
    std::mutex sMutex;

protected:
    bool isConnected(){return device.isConnected();}
    M measurements;
    C configuration;
    bool status;
    D device;
    DeviceBuffer<DeviceCommand<D>> buffer;


    void run_thread() override;
};
#include "opc_controller.hxx"
#endif // OPC_TEMPLATE_CONTROLLER_H
