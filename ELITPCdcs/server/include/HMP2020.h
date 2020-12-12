#ifndef HMP2020_H
#define HMP2020_H

#include "DCSGenericDevice.h"

class HMP2020 : public DCSGenericDevice {
  public:
    HMP2020();
    virtual ~HMP2020();

    std::string getVendor() override;
    std::string getModel() override;
    std::string getSerialNumber() override;
    std::string getPartNumber() override;
    std::string getFirmwareVersion() override;

    // device specific commands
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

  private:
    std::string parseIdentifier(size_t n);
};

#endif  // HMP2020_H
