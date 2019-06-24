#ifndef DT1415ET_H
#define DT1415ET_H

#include <string>
#include <map>
#include "src/utl/GenericDevice.h"

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
    string getStatus(CHANNEL channel);
    string getCurrent(CHANNEL channel);
    string getCurrentSet(CHANNEL channel);

    void setON(CHANNEL channel);
    void setOFF(CHANNEL channel);
    void setVoltageSet(CHANNEL channel, double value);
    void setVoltageMax(CHANNEL channel, string value);

private:
    const std::map<CMD,string> CommandsMap = {{CMD::MON, "MON"},{CMD::SET,"SET"}};
    const std::map<STATUS,string> StatusMap = {{STATUS::CMDOK, "#CMD:OK"},{STATUS::CMDERR,"#CMD:ERR"},
                                               {STATUS::CHERR, "#CH:ERR"},{STATUS::PARERR,"#PAR:ERR"},
                                               {STATUS::VALERR, "#VAL:ERR"},{STATUS::LOCERR,"#LOC:ERR"},
                                               {STATUS::UNKNOWN, "UNKNOWN"}};

    string extractCommandStatus(string command);
    string extractCommandValue(string command);
};

enum class DT1415ETchannelStatus
{
    ON = (1 << 0),
    RUP = (1 << 1),
    RDW = (1 << 2),
    OVC = (1 << 3),
    OVV = (1 << 4),
    UNV = (1 << 5),
    TRIP = (1 << 6),
    OVP = (1 << 7),
    TWN = (1 << 8),
    OVT = (1 << 9),
    KILL = (1 << 10),
    INTLK = (1 << 11),
    ISDIS = (1 << 12),
    FAIL = (1 << 13),
    LOCK = (1 << 14),
    UNKNOWN = (1 << 31)
};

constexpr DT1415ETchannelStatus operator |(DT1415ETchannelStatus a, DT1415ETchannelStatus b)
{
    return static_cast<DT1415ETchannelStatus>(static_cast<int>(a)|static_cast<int>(b));
}

constexpr DT1415ETchannelStatus operator &(DT1415ETchannelStatus a, DT1415ETchannelStatus b)
{
    return static_cast<DT1415ETchannelStatus>(static_cast<int>(a)&static_cast<int>(b));
}

#endif // DT1415ET_H
