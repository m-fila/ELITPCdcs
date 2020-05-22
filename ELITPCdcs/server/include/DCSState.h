
#ifndef DCS_State_CONTROLLER_H
#define DCS_State_CONTROLLER_H
#include "DCSObject.h"
using namespace std::placeholders;

class DCSState : public DCSObject {
  friend DCSServer;

private:
  DCSState(UA_Server *server, std::string name);
  void setState(std::vector<UA_Variant> input, UA_Variant *output);

  enum MachineState : UA_Int32 {
    Idle,
    Pumping,
    Ready,
  } state;
};

#endif // DCS_State_CONTROLLER_H
