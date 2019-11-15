#include "include/piweathercontroller.h"
#include <iostream>
piweather_controller::piweather_controller(std::string OName, QObject *parent):
    opc_controller(OName, parent)
{
}