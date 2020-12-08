#include "mks946controller.h"
#include <iostream>
MKS946_controller::MKS946_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent) {}
