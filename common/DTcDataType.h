#ifndef DTCDATATYPE_H
#define DTCDATATYPE_H
#include <string>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <ostream>
#include <map>
typedef struct{
    UA_Boolean isRemote;
    UA_Float totalVoltageSet;
    UA_Float voltageSet[8];
    UA_Int32 status[8];
    } DTConfiguration;
#define DT_padding_totalVoltageSet offsetof(DTConfiguration,totalVoltageSet) - offsetof(DTConfiguration,isRemote) - sizeof(UA_Boolean)
#define DT_padding_voltageSet offsetof(DTConfiguration,voltageSet) - offsetof(DTConfiguration,totalVoltageSet) - sizeof(UA_Float)
#define DT_padding_status offsetof(DTConfiguration,status) - offsetof(DTConfiguration,voltageSet) - sizeof(UA_Float)*8

struct dtc_customType{
    UA_DataTypeMember members[4] = {
            {
                    UA_TYPENAME("isRemote") /* .memberName */
                    UA_TYPES_BOOLEAN,  /* .memberTypeIndex, points into UA_TYPES since*/
                    0,               /* .padding */
                    true,            /* .namespaceZero, see .memberTypeIndex */
                    false            /* .isArray */
            },
            {
                    UA_TYPENAME("totalVoltageSet")
                    UA_TYPES_FLOAT, DT_padding_totalVoltageSet, true, false
            },
            {
                    UA_TYPENAME("current")
                    UA_TYPES_FLOAT, DT_padding_voltageSet, true, true
            },
            {
                    UA_TYPENAME("status")
                    UA_TYPES_INT32, DT_padding_status, true, true
            }
    };


    const UA_DataType Type = {
            UA_TYPENAME("DTConfiguration")             /* .typeName */
            {1, UA_NODEIDTYPE_NUMERIC, {4244}}, /* .typeId */
            sizeof(DTConfiguration),                   /* .memSize */
            0,                               /* .typeIndex, in the array of custom t*/
            UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
            true,                            /* .pointerFree */
            false,                           /* .overlayable (depends on endianness
                                             the absence of padding) */
            4,                               /* .membersSize */
            2,                               /* .binaryEncodingId, the numeric
                                             identifier used on the wire (the
                                             namespaceindex is from .typeId) */
            members
    };

    UA_DataType Types[1]={Type};

    UA_DataTypeArray DataTypeArray(UA_DataTypeArray *next){
        UA_DataTypeArray customArray={next,1,Types};
        return customArray;
    }

    void addCustomVariableTypeNode(UA_Server *server) {
            char* language=const_cast<char*>("en-Us");
            char* dtcname=const_cast<char*>("DTConfiguration");
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(language, dtcname);
            dattr.displayName = UA_LOCALIZEDTEXT(language, dtcname);
            dattr.dataType = Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;
            DTConfiguration DTConf=get();
            UA_Variant_setScalar(&dattr.value, &DTConf, &Type);
            UA_QualifiedName TypeQName=UA_QUALIFIEDNAME_ALLOC(1, "DTConfiguration");
            UA_Server_addVariableTypeNode(server, Type.typeId,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                          TypeQName,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          dattr, nullptr, nullptr);
            UA_QualifiedName_deleteMembers(&TypeQName);

        }
    DTConfiguration get(bool isRemote,float totalVoltageSet,float voltageSet[],int status[]){
        DTConfiguration dtconf;
        dtconf.isRemote=isRemote;
        dtconf.totalVoltageSet=totalVoltageSet;
        for(int i=0; i<8;++i){
            dtconf.voltageSet[i]=voltageSet[i];
            dtconf.status[i]=status[i];
        }
        return dtconf;
    }

    DTConfiguration get(){
        bool isRemote=false;
        float totalVoltageSet=0;
        float voltageSet[8]={0,0,0,0,0,0,0,0};
        int status[8]={0,0,0,0,0,0,0,0};
        DTConfiguration dtconf=get(isRemote,totalVoltageSet,voltageSet,status);
        return dtconf;
    }
};

static std::ostream& operator<<(std::ostream& os, const DTConfiguration& dtconf){
    os<<dtconf.isRemote<<", "<<dtconf.totalVoltageSet;
    for(int i=0;i<8;++i){
        os<<", "<<dtconf.voltageSet[i];
    }
    for(int i=0;i<8;++i){
        os<<", "<<dtconf.status[i];
    }
    return os;
}

enum class DT1415ETchannelStatus
{   OFF = 0,
    ON = (1 << 0),
    RUP = (1 << 1),
    RDW = (1 << 2),
    OVC = (1 << 3),
    OVV = (1 << 4),
    UNV = (1 << 5),
    TRIP = (1 << 6),
    OVP = (1 << 7),
    TWN = (1 << 8),
    OVT = (1 << 9),
    KILL = (1 << 10),
    INTLK = (1 << 11),
    ISDIS = (1 << 12),
    FAIL = (1 << 13),
    LOCK = (1 << 14),
    UNKNOWN = (1 << 31)
};


constexpr DT1415ETchannelStatus operator |(DT1415ETchannelStatus a, DT1415ETchannelStatus b)
{
    return static_cast<DT1415ETchannelStatus>(static_cast<int>(a)|static_cast<int>(b));
}

constexpr DT1415ETchannelStatus operator &(DT1415ETchannelStatus a, DT1415ETchannelStatus b)
{
    return static_cast<DT1415ETchannelStatus>(static_cast<int>(a)&static_cast<int>(b));
}

#endif // DTCDATATYPE_H
