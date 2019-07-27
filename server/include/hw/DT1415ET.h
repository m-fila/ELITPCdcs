#ifndef DT1415ET_H
#define DT1415ET_H

#include <string>
#include <map>
#include "../utl/GenericDevice.h"

enum class DT1415ETchannelStatus;

class DT1415ET : public GenericDevice
{
public:
    enum class CMD {MON, SET};
    enum class CHANNEL {CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, ALL, NONE};
    enum class STATUS {CMDOK, CMDERR, CHERR, PARERR, VALERR, LOCERR, UNKNOWN};

public:
    DT1415ET();
    virtual ~DT1415ET();
    string sendDT1415ETcommand(CMD command, CHANNEL channel, string function, string value);

    string getModuleName();
    string getFirmwareVersion();
    string getSerialNumber();
    string getControlMode();
    bool isRemote();
    string getIdentifier();
    string getVoltageSet(CHANNEL channel);
    string getVoltage(CHANNEL channel);
    string getVoltageMax(CHANNEL channel);
    string getRampUp(CHANNEL channel);
    string getRampDown(CHANNEL channel);
    string getStatus(CHANNEL channel);
    string getCurrent(CHANNEL channel);
    string getCurrentSet(CHANNEL channel);

    void setON(CHANNEL channel);
    void setOFF(CHANNEL channel);
    void setVoltageSet(CHANNEL channel, double value);
    void setVoltageMax(CHANNEL channel, double value);
    void setRampUp(CHANNEL channel,double value);
    void setRampDown(CHANNEL,double value);

private:
    const std::map<CMD,string> CommandsMap = {{CMD::MON, "MON"},{CMD::SET,"SET"}};
    const std::map<STATUS,string> StatusMap = {{STATUS::CMDOK, "#CMD:OK"},{STATUS::CMDERR,"#CMD:ERR"},
                                               {STATUS::CHERR, "#CH:ERR"},{STATUS::PARERR,"#PAR:ERR"},
                                               {STATUS::VALERR, "#VAL:ERR"},{STATUS::LOCERR,"#LOC:ERR"},
                                               {STATUS::UNKNOWN, "UNKNOWN"}};

    string extractCommandStatus(string command);
    string extractCommandValue(string command);
};



#endif // DT1415ET_H
