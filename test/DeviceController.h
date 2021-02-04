#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "ConnectionParameters.h"
#include "TCPConnector.h"
//#include <iostream>

template <class T> class AbstractDeviceController {

  public:
    AbstractDeviceController() {
        device.reset(new T);
        device->resetConnectionStream();
    }
    void disconnect() { device->resetConnectionStream(); }
    void connect(ConnectionParameters *parameters) {
        auto cp = static_cast<TCPConnectionParameters *>(parameters);
        //    std::cout << cp->IPaddress << ":" << cp->port << std::endl;
        device->setConnectionStream(
            TCPConnector::connect(cp->IPaddress.c_str(), cp->port));
    }
    std::unique_ptr<T> device;
};

#endif  // DEVICECONTROLLER_H
