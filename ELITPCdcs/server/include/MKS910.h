#ifndef MKS910_H
#define MKS910_H

#include "MKS.h"
#include "mks910codes.h"

class MKS910 : public MKS
{
public:
    MKS910();

    //O ok, M Pirani fail, Z Piezo fail
    std::string getStatus();
    //Piezo temperatur +-3 C deg
    std::string getTemp();
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
    void setUnits(MKS910codes::Units u);
};

#endif // MKS910_H
