#ifndef DCS_PIWEATHER_CONTROLLER_H
#define DCS_PIWEATHER_CONTROLLER_H

#include "PiWeather.h"
#include "DCSDeviceController.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"

class DCSPiWeatherController : public DCSDeviceController<PiWeather>{
  friend DCSServer;
private:
  DCSPiWeatherController(UA_Server *server, std::string name);
  UA_PiWeatherm getMeasurements();
};

#endif // DCS_PIWEATHER_CONTROLLER_H