#ifndef MKS_H
#define MKS_H

#include "DCSGenericDevice.h"

class MKS : public DCSGenericDevice {
  public:
    MKS();

    inline void setDeviceDelay(int d) { delay = d; }
    inline void setDeviceAddress(int ad) { address = ad; }

    std::string getUnits();

  protected:
    std::string encapsulate(std::string content);
    std::string extract(std::string content);
    std::string sendWithDelayedResponse(std::string command);
    int delay;
    int address;
};

#endif  // MKS_H
