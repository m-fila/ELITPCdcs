#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "ConnectionParameters.h"
#include "TCPConnector.h"
#include <iostream>

template<class T>
class AbstractDeviceController
{

protected:
    AbstractDeviceController() {device.reset(new T); device->resetConnectionStream();}

    //default connect and disconnect implementations
    void disconnect() {device->resetConnectionStream();}
    void connect(ConnectionParameters* parameters)
    {
        //TODO: use dedicated factory based on device activeConnectionType ?
        TCPConnectionParameters *cp = static_cast<TCPConnectionParameters*>(parameters);
        std::cout << cp->IPaddress  << ":" << cp->port << std::endl;
        device->setConnectionStream(TCPConnector::connect(cp->IPaddress.c_str(), cp->port));
    }

        std::unique_ptr<T> device;
};

#endif // DEVICECONTROLLER_H
