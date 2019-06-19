#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "ConnectionStream.h"
#include <memory>

class GenericDevice
{
public:
    GenericDevice();
    virtual ~GenericDevice();

    void setConnectionStream(ConnectionStream* stream);
    void resetConnectionStream();
    bool isConnected() {return connected;}
    void sendCommand(string command);
    string sendWithResponse(string command);
    bool connected;

protected:
    GenericDevice(ConnectionType allowed, ConnectionType active): connectionTypeAllowed(allowed), activeConnectionType(active) {}
    ConnectionType connectionTypeAllowed;
    ConnectionType activeConnectionType;
    ConnectionStreamPtr connectionStream;

};

#endif // GENERICDEVICE_H
