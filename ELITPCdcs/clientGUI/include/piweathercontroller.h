#ifndef PIWEATHER_CONTROLLER_H
#define PIWEATHER_CONTROLLER_H
#include "opc_controller.h"
class piweather_controller : public opc_controller {
    Q_OBJECT
  public:
    piweather_controller(std::string OName, QObject *parent = 0);
};

#endif  // PIWEATHER_CONTROLLER_H
