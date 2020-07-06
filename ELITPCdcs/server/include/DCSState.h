
#ifndef DCS_State_CONTROLLER_H
#define DCS_State_CONTROLLER_H
#include "DCSObject.h"

class DCSState : public DCSObject {
  friend DCSServer;

private:
  DCSState() = default;
  void addChildren() override;
  void setState(const UA_Variant *input, UA_Variant *output);

  enum MachineState : UA_Int32 {
    Idle,
    Pumping,
    Ready,
  } state;
  static std::string GetType() { return "DCSState"; }
};

#endif // DCS_State_CONTROLLER_H
