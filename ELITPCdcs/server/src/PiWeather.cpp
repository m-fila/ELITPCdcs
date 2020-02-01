#include "PiWeather.h"
#include <sstream>
#include <iostream>
PiWeather::PiWeather() : GenericDevice(ConnectionType::TCP | ConnectionType::Serial, ConnectionType::TCP)
{

}

PiWeather::~PiWeather()
{

}

std::string PiWeather::getMeasurements()
{
    return sendWithResponse("GET");
}
