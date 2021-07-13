#include "tpgcontroller.h"
#include <iostream>
tpg_controller::tpg_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent) {}

void tpg_controller::callSetRelay(int nr, int enabled, double setpoint,
                                  double hysteresis) {
    UA_Variant input[4];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[1], &enabled, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[2], &setpoint, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[3], &hysteresis, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds["setrelay"], 4, input, nullptr,
                   nullptr);
    UA_Variant_clear(input);
}

void tpg_controller::callSetStatus(int channel, bool isOn){
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &channel, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[1], &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_call(client, ObjectNodeId, browsedIds["setstatus"], 2, input, nullptr,
                   nullptr);
    UA_Variant_clear(input);
}
