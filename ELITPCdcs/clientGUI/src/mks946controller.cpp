#include "mks946controller.h"
#include <iostream>
MKS946_controller::MKS946_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent) {}

void MKS946_controller::callSetRelay(int nr, int enabled, double setpoint,
                                     double hysteresis) {
    UA_Variant input[4];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[1], &enabled, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[2], &setpoint, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[3], &hysteresis, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds["setrelay"], 4, input, nullptr,
                         nullptr, nullptr);
    UA_Variant_clear(input);
}