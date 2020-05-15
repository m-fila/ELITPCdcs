#include "GenericDevice.h"
#include <iostream>
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
    std::cout<<"dis"<<std::endl;
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
}

std::string GenericDevice::sendWithResponse(std::string command)
{
    if(isConnected())
    {
        return connectionStream->sendWithResponse(command);
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
}