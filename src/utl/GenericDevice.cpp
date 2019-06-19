#include "GenericDevice.h"

GenericDevice::GenericDevice(): connected(false)
{

}

GenericDevice::~GenericDevice()
{

}

void GenericDevice::setConnectionStream(ConnectionStream* stream)
{
    connectionStream.reset(stream);
    connected = true;
}

void GenericDevice::resetConnectionStream()
{
    connectionStream.reset();
    connected = false;
}

void GenericDevice::sendCommand(string command)
{
    if(isConnected())
    {
        connectionStream->send(command);
    }
}

string GenericDevice::sendWithResponse(string command)
{
    if(isConnected())
    {
        return connectionStream->sendWithResponse(command);
    }
}
