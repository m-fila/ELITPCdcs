#ifndef LVPSUCONTROLLER_H
#define LVPSUCONTROLLER_H

#include "../DeviceController.h"
#include "../utl/TCPConnector.h"
//#include "HMPseriesContainers.h"
#include "../hw/HMP2020.h"
#include "../../lib/open62541.h"
#include "hmpcontainers.h"
class LVpsuController : public AbstractDeviceController<HMP2020>
{
 //   Q_OBJECT

public:
    LVpsuController();
    virtual ~LVpsuController() {}

//signals:
//    void settingsReadFromDevice(HMPseriesSettings hmpSettings);
//    void measurementsReadFromDevice(HMPseriesMeasurements hmpMeasurements);

//public slots:
//    void setCH1outputON();
//    void setCH1outputOFF();
//    void setCH2outputON();
//    void setCH2outputOFF();
//    void setGENoutputON();
//    void setGENoutputOFF();

private:
   void run();
   int ReadoutPeriod;

//    QQueue<Command> commandQueue;

    HMPSeries getMeasurements();
    void setCH1outputON();
    void setCH1outputOFF();
    void setCH2outputON();
    void setCH2outputOFF();
    void setGENoutputON();
    void setGENoutputOFF();
};

#endif // LVPSUCONTROLLER_H
