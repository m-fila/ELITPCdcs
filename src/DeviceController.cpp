#include "src/DeviceController.h"
#include <iostream>
#include <stdexcept>
DeviceController::DeviceController() : stopLoop(true), connectionStatus(false)
{

}

DeviceController::~DeviceController()
{

}

void DeviceController::setStop()
{
    stopLoop = true;
}
/*
void DeviceController::deviceConnect(ConnectionParameters* parameters)
{
    std::cout << "Connecting to device ..." << std::endl;
    //if connection succeded emit signal
    try{
        connect(parameters);
        emit connected();
        connectionStatus=true;
        //std::cout << "Connected!" << std::endl;

    }catch(std::runtime_error &e){
        QString message("ERROR: ");
        message.append(e.what());

        emit statusChanged(message);
    }
}

void DeviceController::deviceDisconnect()
{
    if(!stopLoop)
    {
        setStop();
        while(!isStopped) {;;}
    }

    if(connectionStatus)
    {
        std::cout << "Disconnecting from device ..." << std::endl;
        disconnect();
        connectionStatus=false;
    }
   // emit disconnected();
}
*/
