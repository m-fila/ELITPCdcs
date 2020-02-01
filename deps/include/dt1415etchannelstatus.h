#ifndef DT1415ETCHANNELSTATUS
#define DT1415ETCHANNELSTATUS

enum class DT1415ETchannelStatus
{   OFF = 0,
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

#endif // DT1415ETCHANNELSTATUS

