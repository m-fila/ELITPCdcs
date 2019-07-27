#ifndef HMP2020_H
#define HMP2020_H

#include "../utl/GenericDevice.h"

class HMP2020 : public GenericDevice
{
public:
    HMP2020();
    virtual ~HMP2020();

    //device specific commands
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
};

#endif // HMP2020_H
