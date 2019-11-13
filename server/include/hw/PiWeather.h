#ifndef PIWEATHER_H
#define PIWEATHER_H

#include "../utl/GenericDevice.h"

class PiWeather : public GenericDevice
{
public:
    PiWeather();
    virtual ~PiWeather();

    //device specific commands
    std::string getMeasurements();

};

#endif // PIWEATHER_H
