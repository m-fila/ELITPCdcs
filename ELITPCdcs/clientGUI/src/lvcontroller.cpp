#include "lvcontroller.h"
#include <iostream>
lv_controller::lv_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent), setChannelBrowseName("setchannel"),
      setOutputBrowseName("setoutput"), setVoltageBrowseName("setvoltage"),
      setCurrentBrowseName("setcurrent") {}
void lv_controller::callSetOutput(bool state) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setOutputBrowseName], 1, &input,
                   nullptr, nullptr);
    UA_Variant_clear(&input);
}

void lv_controller::callSetChannel(int nr, bool state) {
    //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setChannelBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void lv_controller::callSetVoltage(int nr, double v) {
    //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &v, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setVoltageBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void lv_controller::callSetCurrent(int nr, double i) {
    //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &i, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setCurrentBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}
