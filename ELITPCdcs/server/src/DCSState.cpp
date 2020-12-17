#include "DCSState.h"
void DCSState::addChildren(const Options &options) {
    auto &v = addVariable("State", &UA_TYPES[UA_TYPES_UINT32]);
    v.setValue(0);
    addMethod("setstate", "Changes state of machine",
              {{"state", "IDLE/FILLING/READY", &UA_TYPES[UA_TYPES_UINT32]}}, {},
              std::bind(&::DCSState::setState, this, std::placeholders::_1,
                        std::placeholders::_2));
}

void DCSState::setState(const UA_Variant *input, UA_Variant *output) {
    MachineState state = *static_cast<MachineState *>(input[0].data);
    variables.at("State")->setValue(state);
}
