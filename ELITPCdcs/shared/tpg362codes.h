#ifndef TPG362CODES_H
#define TPG362CODES_H
#include <map>
#include <string>

namespace TPG362codes{

    enum class Status{
    OK,
    Underrange,
    Overrange,
    SensorError,
    SensorOff,
    NoSensor,
    IdentificationError
    };

    static std::map<Status, std::string> statusToString={
    {Status::OK,"Measurement data okay"},
    {Status::Underrange,"Underrange"},
    {Status::Overrange,"Overrange"},
    {Status::SensorError,"Sensor error"},
    {Status::SensorOff,"Sensor off"},
    {Status::NoSensor,"No sensor"},
    {Status::IdentificationError,"Identification error"}
    };

    enum class Units{   
        MBAR,TORR,PASCAL,MICRON,HPASCAL,VOLT
    };

    static std::map<Units, std::string> unitsToString={
        {Units::MBAR,"MBAR"},
        {Units::TORR,"TORR"},
        {Units::PASCAL, "PASCAL"},
        {Units::MICRON,"MICRON"},
        {Units::HPASCAL,"HPASCAL"},
        {Units::VOLT,"VOLT"},
    };
    static std::map<std::string, Units> unitsFromString={
        {"MBAR",Units::MBAR},
        {"TORR",Units::TORR},
        {"PASCAL",Units::PASCAL},
        {"MICRON",Units::MICRON,},
        {"HPASCAL",Units::HPASCAL,},
        {"VOLT",Units::VOLT},

    };
}
#endif // TPG362CODES_H

