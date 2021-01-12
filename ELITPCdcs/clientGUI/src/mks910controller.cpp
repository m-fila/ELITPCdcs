#include "mks910controller.h"
#include <iostream>
MKS910_controller::MKS910_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent) {}

void MKS910_controller::callSetUnits(int unit) {
    //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &unit, &UA_TYPES[UA_TYPES_INT32]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setUnitsBrowseName], 1, &input,
                   nullptr, nullptr);
    UA_Variant_clear(&input);
}

void MKS910_controller::callSetRelay(int nr, int enabled, double setpoint,
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
