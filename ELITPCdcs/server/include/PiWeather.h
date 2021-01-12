#ifndef PIWEATHER_H
#define PIWEATHER_H

#include "DCSGenericDevice.h"

class PiWeather : public DCSGenericDevice {
  public:
    PiWeather();

    // device specific commands
    std::string getMeasurements();
};

#endif  // PIWEATHER_H
