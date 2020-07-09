#ifndef DCS_PIWEATHER_CONTROLLER_H
#define DCS_PIWEATHER_CONTROLLER_H

#include "DCSDeviceController.h"
#include "PiWeather.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"

class DCSPiWeatherController : public DCSDeviceController<PiWeather>,
                               DCSObjectFactory::Register<DCSPiWeatherController> {
    friend DCSServer;
    friend DCSObjectFactory;

  private:
    static std::string GetType() { return "PiWeather"; }
    DCSPiWeatherController(){};
    void addChildren(Options options) override;
    UA_PiWeatherm getMeasurements();
};
// REGISTER_OBJECT(DCSPiWeatherController)
#endif  // DCS_PIWEATHER_CONTROLLER_H
