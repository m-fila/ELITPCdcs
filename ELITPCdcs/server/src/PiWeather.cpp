#include "PiWeather.h"

#include <sstream>
PiWeather::PiWeather()
    : DCSGenericDevice(ConnectionType::TCP | ConnectionType::Serial,
                       ConnectionType::TCP) {}

std::string PiWeather::getMeasurements() { return sendWithResponse("GET"); }
