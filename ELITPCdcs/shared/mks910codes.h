#ifndef MKS910CODES_H
#define MKS910CODES_H
#include <map>
#include <string>

namespace MKS910codes{
    enum class Units{   
        PASCAL,MBAR,TORR
    };

    static std::map<Units, std::string> unitsToString={
        {Units::PASCAL, "PASCAL"},
        {Units::MBAR,"MBAR"},
        {Units::TORR,"TORR"}
    };

    static std::map<std::string, Units> unitsFromString={
        {"PASCAL",Units::PASCAL},
        {"MBAR",Units::MBAR},
        {"TORR",Units::TORR}
    };
}
#endif // MKS910CODES_H

