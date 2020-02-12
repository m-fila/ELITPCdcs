#ifndef PIWEATHERCONTROLLER_H
#define PIWEATHERCONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "PiWeather.h"
#include "opc_controller.h"

#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class PiWeatherController :public OpcTemplateController<UA_PiWeatherm,UA_PiWeatherc,PiWeather>
{
public:
    PiWeatherController(std::string name);
//    ~PiWEATHERController();
    void init(UA_Server *server);

private:
    UA_PiWeatherm getMeasurements() final;
    UA_PiWeatherc getSettings() final;
};
#endif // PIWEATHERCONTROLLER_H
