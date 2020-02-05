#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "ConnectionStream.h"
#include <memory>
#include <chrono>
#include <thread>
class GenericDevice
{
public:
    GenericDevice();
    virtual ~GenericDevice();

    void setConnectionStream(ConnectionStream* stream);
    void resetConnectionStream();
    bool isConnected() {return connected;}
    void sendCommand(std::string command);
    std::string receiveResponse();
    std::string sendWithResponse(std::string command);
    std::string sendWithDelayedResponse(std::string command, uint delay);
    ConnectionType getActiveConnectionType() {return activeConnectionType;}

protected:
    GenericDevice(ConnectionType allowed, ConnectionType active): connectionTypeAllowed(allowed), activeConnectionType(active) {}
    ConnectionType connectionTypeAllowed;
    ConnectionType activeConnectionType;
    ConnectionStreamPtr connectionStream;
    bool connected;
};

#endif // GENERICDEVICE_H
