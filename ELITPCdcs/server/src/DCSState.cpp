#include "DCSState.h"
DCSState::DCSState(UA_Server *server, std::string name)
    : DCSObject(server, "MachineState") {
  auto &v=addVariable("State", UA_TYPES[UA_TYPES_UINT32]);
  v.setValue(0);
  addMethod("setstate", "Changes state of machine", {{"state", "IDLE/FILLING/READY", UA_TYPES[UA_TYPES_UINT32]}}, {},
            std::bind(&::DCSState::setState, this,_1,_2));
}

void DCSState::setState(std::vector<UA_Variant> input, UA_Variant *output) {
    MachineState state = *static_cast<MachineState*>(input.at(0).data);
    variables.at("State")->setValue(state);
}