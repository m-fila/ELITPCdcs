#ifndef N1471CODES_H
#define N1471CODES_H
#include <map>
#include <string>

namespace N1471codes{
    enum class ChannelStatus{
        OFF = 0,
        ON = (1 << 0),
        RUP = (1 << 1),
        RDW = (1 << 2),
        OVC = (1 << 3),
        OVV = (1 << 4),
        UNV = (1 << 5),
        MAXV = (1 << 6),
        TRIP = (1 << 7),
        OVP = (1 << 8),
        OVT = (1 << 9),
        DIS = (1 << 10),
        KILL = (1 << 11),
        ILK = (1 << 12),
        NOCAL = (1 << 13),
        UNKNOWN=(1<<31)
    };


    static constexpr ChannelStatus operator |(ChannelStatus a, ChannelStatus b){
        return static_cast<ChannelStatus>(static_cast<int>(a)|static_cast<int>(b));
    }

    static constexpr ChannelStatus operator &(ChannelStatus a, ChannelStatus b){
        return static_cast<ChannelStatus>(static_cast<int>(a)&static_cast<int>(b));
    }

    static std::map<ChannelStatus,std::string> channelStatusToString = {
    {ChannelStatus::OFF, "OFF"},
    {ChannelStatus::ON, "ON"},
    {ChannelStatus::RUP,"RUP"},
    {ChannelStatus::RDW,"RDW"},
    {ChannelStatus::OVC,"OVC"},
    {ChannelStatus::OVV,"OVV"},
    {ChannelStatus::UNV,"UNV"},
    {ChannelStatus::MAXV,"MAXV"},
    {ChannelStatus::TRIP,"TRIP"},
    {ChannelStatus::OVP,"OVP"},
    {ChannelStatus::OVT,"OVT"},
    {ChannelStatus::DIS,"DIS"},
    {ChannelStatus::KILL,"KILL"},
    {ChannelStatus::ILK,"ILK"},
    {ChannelStatus::NOCAL,"NOCAL"},
    {ChannelStatus::UNKNOWN,"UNKNOWN"}
    };

    static std::string translateChannelStatus(ChannelStatus status){
        std::string str="";
        for(int i=0;i<14;++i){
            ChannelStatus current_status=static_cast<ChannelStatus>(1<<i);
            if(static_cast<bool>(status & current_status)){
                str+=" ";
                str+=channelStatusToString.at(current_status);
            }
        }
        if(status==ChannelStatus::OFF){
            str=channelStatusToString.at(ChannelStatus::OFF);
        }
        return str;
    }

}
#endif // N1471CODES_H

