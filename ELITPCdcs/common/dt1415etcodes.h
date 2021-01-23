#ifndef DT1415ETCODES_H
#define DT1415ETCODES_H
#include <map>
#include <string>

namespace DT1415ETcodes {
enum class ChannelStatus {
    OFF = 0,
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

static constexpr ChannelStatus operator|(ChannelStatus a, ChannelStatus b) {
    return static_cast<ChannelStatus>(static_cast<int>(a) | static_cast<int>(b));
}

static constexpr ChannelStatus operator&(ChannelStatus a, ChannelStatus b) {
    return static_cast<ChannelStatus>(static_cast<int>(a) & static_cast<int>(b));
}

static std::map<ChannelStatus, std::string> channelStatusToString = {
    {ChannelStatus::OFF, "OFF"},        {ChannelStatus::ON, "ON"},
    {ChannelStatus::RUP, "RUP"},        {ChannelStatus::RDW, "RDW"},
    {ChannelStatus::OVC, "OVC"},        {ChannelStatus::OVV, "OVV"},
    {ChannelStatus::UNV, "UNV"},        {ChannelStatus::TRIP, "TRIP"},
    {ChannelStatus::OVP, "OVP"},        {ChannelStatus::TWN, "TWN"},
    {ChannelStatus::OVT, "OVT"},        {ChannelStatus::KILL, "KILL"},
    {ChannelStatus::INTLK, "INTLK"},    {ChannelStatus::ISDIS, "ISDIS"},
    {ChannelStatus::FAIL, "FAIL"},      {ChannelStatus::LOCK, "LOCK"},
    {ChannelStatus::UNKNOWN, "UNKNOWN"}};

static std::string translateChannelStatus(ChannelStatus status) {
    std::string str = "";
    for(int i = 0; i < 15; ++i) {
        ChannelStatus current_status = static_cast<ChannelStatus>(1 << i);
        if(static_cast<bool>(status & current_status)) {
            str += " ";
            str += channelStatusToString.at(current_status);
        }
    }
    if(status == ChannelStatus::OFF) {
        str = channelStatusToString.at(ChannelStatus::OFF);
    }
    return str;
}

}  // namespace DT1415ETcodes
#endif  // DT1415ETCODES_H
