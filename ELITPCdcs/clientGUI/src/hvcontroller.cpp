#include "hvcontroller.h"
#include <iostream>
hv_controller::hv_controller(std::string OName, QObject *parent)
    : opc_controller(OName, parent), setChannelBrowseName("setchannel"),
      setVoltageBrowseName("setvoltage"), setVoltageMaxBrowseName("setvoltagemax"),
      setRampUpBrowseName("setrampup"), setRampDownBrowseName("setrampdown"),
      setCurrentBrowseName("setcurrent"), clearAlarmBrowseName("clearalarm") {}

void hv_controller::callSetChannel(int nr, bool state) {
    //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &state, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setChannelBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetVoltage(int nr, double voltage) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &voltage, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setVoltageBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetVoltageMax(int nr, double voltage) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &voltage, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setVoltageMaxBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetRampUp(int nr, double rup) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &rup, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setRampUpBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetRampDown(int nr, double rdwn) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &rdwn, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setRampDownBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callSetCurrent(int nr, double current) {
    //   UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetOutput"));
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_Variant_setScalarCopy(&input[0], &nr, &UA_TYPES[UA_TYPES_INT16]);
    UA_Variant_setScalarCopy(&input[1], &current, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Client_call(client, ObjectNodeId, browsedIds[setCurrentBrowseName], 2, input,
                   nullptr, nullptr);
    UA_Variant_clear(input);
}

void hv_controller::callClearAlarm() {
    UA_Client_call(client, ObjectNodeId, browsedIds[clearAlarmBrowseName], 0, nullptr,
                   nullptr, nullptr);
}