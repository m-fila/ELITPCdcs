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

void MKS946_controller::callConfigurePID(
    std::string flowChannel, std::string pressureChannel, double pressureSetpoint,
    double kp, double timeConstant, double derivativeTimeConstant, double ceiling,
    double base, double preset, double start, double end, double ctrlStart,
    std::string direction, int band, int gain) {
    UA_Variant input[15];
    UA_Variant_init(input);
    auto flowStr = UA_STRING_ALLOC(flowChannel.c_str());
    auto pressureStr = UA_STRING_ALLOC(pressureChannel.c_str());
    auto directionStr = UA_STRING_ALLOC(direction.c_str());
    UA_Variant_setScalarCopy(&input[0], &flowStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &pressureStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[2], &pressureSetpoint, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[3], &kp, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[4], &timeConstant, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[5], &derivativeTimeConstant,
                             &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[6], &ceiling, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[7], &base, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[8], &preset, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[9], &start, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[10], &end, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[11], &ctrlStart, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[12], &directionStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[13], &band, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Variant_setScalarCopy(&input[14], &gain, &UA_TYPES[UA_TYPES_UINT32]);

    UA_Client_call_async(client, ObjectNodeId, browsedIds["configurePID"], 15, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void MKS946_controller::callConfigureFlow(std::string mode, double setpoint,
                                          double nominalRange, double scaleFactor) {
    UA_Variant input[4];
    UA_Variant_init(input);
    auto modeStr = UA_STRING_ALLOC(mode.c_str());
    UA_Variant_setScalarCopy(&input[0], &modeStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &setpoint, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[2], &nominalRange, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[3], &scaleFactor, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds["configureMFC"], 4, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void MKS946_controller::callConfigurePressure(std::string type, double nominalRange,
                                              std::string voltageRange) {
    UA_Variant input[3];
    UA_Variant_init(input);
    auto typeStr = UA_STRING_ALLOC(type.c_str());
    auto volStr = UA_STRING_ALLOC(voltageRange.c_str());
    UA_Variant_setScalarCopy(&input[0], &typeStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &nominalRange, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalarCopy(&input[2], &volStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds["configureManometer"], 3, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void MKS946_controller::callSetPIDState(bool state) {
    UA_Variant input[1];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds["setPIDState"], 1, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void MKS946_controller::callZeroMFC() {
    UA_Client_call_async(client, ObjectNodeId, browsedIds["zeroMFC"], 0, nullptr, nullptr,
                         nullptr, nullptr);
}

void MKS946_controller::callConfigureInterlock(bool enabled, double limit) {
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &enabled, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Variant_setScalarCopy(&input[1], &limit, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds["configureInterlock"], 2, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}
