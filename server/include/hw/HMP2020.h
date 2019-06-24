#ifndef HMP2020_H
#define HMP2020_H

#include "include/utl/GenericDevice.h"

class HMP2020 : public GenericDevice
{
public:
    HMP2020();
    virtual ~HMP2020();

    //device specific commands
    string getIdentifier();
    void setActiveChannel(int channel);
    string getVoltageSet();
    string getCurrentSet();
    string getOutputSel();
    string getOutputGen();
    string getVoltage();
    string getCurrent();
    void setOutputSel(int channel, bool state);
    void setOutputGen(bool state);
};

#endif // HMP2020_H
