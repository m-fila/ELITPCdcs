#include "mks910controller.h"
#include <iostream>
MKS910_controller::MKS910_controller(std::string OName, QObject *parent):
    opc_controller(OName, parent),
    setUnitsBrowseName("setunits")
{
}



void MKS910_controller::callSetUnits(int unit){
  //  UA_NodeId MethodNodeId=UA_NODEID_STRING(1,const_cast<char*>("SetChannel"));
    UA_Variant input;
    UA_Variant_init(&input);
    UA_Variant_setScalarCopy(&input, &unit, &UA_TYPES[UA_TYPES_INT32]);
    UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
                                browsedIds[setUnitsBrowseName], 1, &input, nullptr,nullptr);
    UA_Variant_clear(&input);
}
