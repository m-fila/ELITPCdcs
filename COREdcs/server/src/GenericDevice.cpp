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

void GenericDevice::sendCommand(std::string command)
{
    if(isConnected())
    {
        connectionStream->send(command);
    }
    
}

std::string GenericDevice::receiveResponse(){
       if(isConnected())
    {
        return connectionStream->receive();
    } 
        else{
        return "";
    }
}

std::string GenericDevice::sendWithResponse(std::string command)
{
    if(isConnected())
    {
        return connectionStream->sendWithResponse(command);
    }
    else{
        return "";
    }
}


std::string GenericDevice::sendWithDelayedResponse(std::string command,uint delay)
{
    if(isConnected())
    {   
        connectionStream->send(command);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        return connectionStream->receive();
    }
    else{
        return "";
    }
}