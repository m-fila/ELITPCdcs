#include "PiWeather.h"

#include <sstream>
PiWeather::PiWeather()
    : GenericDevice(ConnectionType::TCP | ConnectionType::Serial, ConnectionType::TCP) {}

std::string PiWeather::getMeasurements() { return sendWithResponse("GET"); }
