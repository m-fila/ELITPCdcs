#ifndef DTMDATATYPE_H
#define DTMDATATYPE_H
#include <string>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <ostream>
typedef struct{
    UA_Float totalVoltage;
    UA_Float voltage[8];
    UA_Float current[8];
    } DTMeasurements;
#define DT_padding_voltage offsetof(DTMeasurements,voltage) - offsetof(DTMeasurements,totalVoltage) - sizeof(UA_Float)
#define DT_padding_current offsetof(DTMeasurements,current) - offsetof(DTMeasurements,voltage) - sizeof(UA_Float)*8


struct dtm_customType{
    UA_DataTypeMember members[3] = {
            {
                    UA_TYPENAME("totalVoltage") /* .memberName */
                    UA_TYPES_FLOAT,  /* .memberTypeIndex, points into UA_TYPES since*/
                    0,               /* .padding */
                    true,            /* .namespaceZero, see .memberTypeIndex */
                    false            /* .isArray */
            },
            {
                    UA_TYPENAME("voltage")
                    UA_TYPES_FLOAT, DT_padding_voltage, true, true
            },
            {
                    UA_TYPENAME("current")
                    UA_TYPES_FLOAT, DT_padding_current, true, true
            }
    };


    const UA_DataType Type = {
            UA_TYPENAME("DTMeasurements")             /* .typeName */
            {1, UA_NODEIDTYPE_NUMERIC, {4243}}, /* .typeId */
            sizeof(DTMeasurements),                   /* .memSize */
            0,                               /* .typeIndex, in the array of custom t*/
            UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
            true,                            /* .pointerFree */
            false,                           /* .overlayable (depends on endianness
                                             the absence of padding) */
            3,                               /* .membersSize */
            1,                               /* .binaryEncodingId, the numeric
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
            char* dtmname=const_cast<char*>("DTMeasurements");
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(language, dtmname);
            dattr.displayName = UA_LOCALIZEDTEXT(language, dtmname);
            dattr.dataType = Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;
            DTMeasurements dtmeasure=get();
            UA_Variant_setScalar(&dattr.value, &dtmeasure, &Type);
            UA_QualifiedName TypeQName=UA_QUALIFIEDNAME_ALLOC(1, "DTMeasurements");
            UA_Server_addVariableTypeNode(server, Type.typeId,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                          TypeQName,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          dattr, nullptr, nullptr);
            UA_QualifiedName_deleteMembers(&TypeQName);
    }

    DTMeasurements get(float tot, float volt[], float  curr[]){
        DTMeasurements dtmeasure;
        dtmeasure.totalVoltage=tot;
        for (int i=0; i<8;++i){
            dtmeasure.voltage[i]=volt[i];
            dtmeasure.current[i]=curr[i];
        }
        return dtmeasure;
    }

    DTMeasurements get(){
        float tot=0;
        float vol[8]={0,0,0,0,0,0,0,0};
        float cur[8]={0,0,0,0,0,0,0,0};
        DTMeasurements dtmeasure=get(tot,vol,cur);
        return dtmeasure;
    }


};

static std::ostream& operator<<(std::ostream& os, const DTMeasurements& dtm){
    os<<dtm.totalVoltage;
    for (int i=0;i<8;++i){
    os<<", "<<dtm.voltage[i];
    }
    for (int i=0;i<8;++i){
    os<<", "<<dtm.current[i];
    }
    return os;
}

#endif // DTMDATATYPE_H
