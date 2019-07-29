#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

//#include <QThread>
//#include <memory>
//#include "utl/TCPStream.h"
#include "ConnectionParameters.h"
#include "utl/TCPConnector.h"
#include <iostream>
class DeviceController //: public QThread
{
//    Q_OBJECT

public:
    DeviceController();
    virtual ~DeviceController();

//signals:
//    void connected();
//    void disconnected();
//    void statusChanged(QString info);

//public slots:
//    void deviceConnect(ConnectionParameters* parameters);
//    void deviceDisconnect();

protected:
    volatile bool stopLoop;
    volatile bool isStopped;
    void setStop();

    bool connectionStatus;
    virtual void connect(ConnectionParameters*) {}
    virtual void disconnect() = 0;
};

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
