#ifndef HMP2020_H
#define HMP2020_H

#include "DCSGenericDevice.h"

class HMP2020 : public DCSGenericDevice {
  public:
    HMP2020();
    virtual ~HMP2020();

    // device specific commands
    std::string getIdentifier();
    void setActiveChannel(int channel);
    std::string getVoltageSet();
    std::string getCurrentSet();
    std::string getOutputSel();
    std::string getOutputGen();
    std::string getVoltage();
    std::string getCurrent();
    void setOutputSel(int channel, bool state);
    void setOutputGen(bool state);
    void setVoltage(int channe, double v);
    void setCurrent(int channel, double i);
};

#endif  // HMP2020_H
