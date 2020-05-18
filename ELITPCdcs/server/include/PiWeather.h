#ifndef PIWEATHER_H
#define PIWEATHER_H

#include "GenericDevice.h"

class PiWeather : public GenericDevice {
public:
  PiWeather();

  // device specific commands
  std::string getMeasurements();
};

#endif // PIWEATHER_H
