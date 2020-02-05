#ifndef MKS910CODES_H
#define MKS910CODES_H
#include <map>
#include <string>

namespace MKS910codes{
enum class MKS910UNITS
{   PASCAL,MBAR,TORR
};

std::map<MKS910UNITS, std::string> unitsToString={
    {MKS910UNITS::PASCAL, "PASCAL"},
    {MKS910UNITS::MBAR,"MBAR"},
    {MKS910UNITS::TORR,"TORR"}
};
std::map<std::string, MKS910UNITS> unitsFromString={
    {"PASCAL",MKS910UNITS::PASCAL},
    {"MBAR",MKS910UNITS::MBAR},
    {"TORR",MKS910UNITS::TORR}
};
}
#endif // MKS910CODES_H

