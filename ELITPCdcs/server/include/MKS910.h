#ifndef MKS910_H
#define MKS910_H

#include "GenericDevice.h"
#include "mks910codes.h"

class MKS910 : public GenericDevice
{
public:
    MKS910();

    enum class UNIT {mbar, torr, pascal};
    inline void setDeviceDelay(int d){delay=d;}
    inline void setDeviceChannel(int ch){channel=ch;}

     std::string sendWithDelayedResponse(std::string command);
    //device specific commands

    //O ok, M Pirani fail, Z Piezo fail
    std::string getStatus();
    //Piezo temperatur +-3 C deg
    std::string getTemp();
    std::string getUnits();
    std::string getPPirani();
    std::string getPPiezo();
    std::string getPCombined();
    std::string getPCombinedLong();
    std::string getPDiff();

    std::string getFirmware();
    std::string getHardware();
    std::string getName();
    std::string getModel();
    std::string getSerialNumber();
    std::string getPartNumer();
    std::string getUptime();
    // TORR, PASCAL, MBAR
    void setUnits(MKS910codes::MKS910UNITS u);
    
    protected:
    std::string encapsulate(std::string content);
    std::string extract(std::string content);
    int delay;
    int channel;
};

#endif // MKS910_H
