#ifndef MKS910_H
#define MKS910_H

#include "MKS.h"
#include "mks910codes.h"

class MKS910 : public MKS {
  public:
    MKS910();
    enum class RelayNo { R1 = 1, R2, R3, R4 };
    // O ok, M Pirani fail, Z Piezo fail
    std::string getStatus();
    // Piezo temperatur +-3 C deg
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

    std::string getRelayStatus(RelayNo i);
    std::string getRelayDirection(RelayNo i);
    std::string getRelayEnabled(RelayNo i);
    std::string getRelaySetPoint(RelayNo i);
    std::string getRelayHysteresis(RelayNo i);
    std::string getRelaySafetyDelay();

    void setRelayDirection(RelayNo i, MKS910codes::RelayDirection dir);
    void setRelayEnabled(RelayNo i, bool isEnabled);
    void setRelaySetpoint(RelayNo i, double v);
    void setRelayHysteresis(RelayNo i, double v);
    void setRelaySafetyDelay(MKS910codes::RelayDelay delay);
};

#endif  // MKS910_H
