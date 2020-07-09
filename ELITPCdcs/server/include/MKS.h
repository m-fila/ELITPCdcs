#ifndef MKS_H
#define MKS_H

#include "GenericDevice.h"

class MKS : public GenericDevice {
  public:
    MKS();

    inline void setDeviceDelay(int d) { delay = d; }
    inline void setDeviceChannel(int ch) { channel = ch; }

    std::string getUnits();

  protected:
    std::string encapsulate(std::string content);
    std::string extract(std::string content);
    std::string sendWithDelayedResponse(std::string command);
    int delay;
    int channel;
};

#endif  // MKS_H
