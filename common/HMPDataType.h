#ifndef HMPMeasurementsCONTAINERS_H
#define HMPMeasurementsCONTAINERS_H



#include <string>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <ostream>
typedef struct{
    UA_Boolean CH1;
    UA_Boolean CH2;
    UA_Boolean Output;
    UA_Float CH1_voltage;
    UA_Float CH2_voltage;
    UA_Float CH1_current;
    UA_Float CH2_current;
    } HMPMeasurements;
#define HMP_padding_CH2 offsetof(HMPMeasurements,CH2) - offsetof(HMPMeasurements,CH1) - sizeof(UA_Boolean)
#define HMP_padding_Output offsetof(HMPMeasurements,Output) - offsetof(HMPMeasurements,CH2) - sizeof(UA_Boolean)
#define HMP_padding_CH1_voltage offsetof(HMPMeasurements,CH1_voltage) - offsetof(HMPMeasurements,Output) - sizeof(UA_Boolean)
#define HMP_padding_CH2_voltage offsetof(HMPMeasurements,CH2_voltage) - offsetof(HMPMeasurements,CH1_voltage) - sizeof(UA_Float)
#define HMP_padding_CH1_current offsetof(HMPMeasurements,CH1_current) - offsetof(HMPMeasurements,CH2_voltage) - sizeof(UA_Float)
#define HMP_padding_CH2_current offsetof(HMPMeasurements,CH2_current) - offsetof(HMPMeasurements,CH1_current) - sizeof(UA_Float)

struct hmp_customType{
    UA_DataTypeMember members[7] = {
            {
                    UA_TYPENAME("CH1") /* .memberName */
                    UA_TYPES_BOOLEAN,  /* .memberTypeIndex, points into UA_TYPES since*/
                    0,               /* .padding */
                    true,            /* .namespaceZero, see .memberTypeIndex */
                    false            /* .isArray */
            },
            {
                    UA_TYPENAME("CH2")
                    UA_TYPES_BOOLEAN, HMP_padding_CH2, true, false
            },
            {
                    UA_TYPENAME("OUTPUT")
                    UA_TYPES_BOOLEAN, HMP_padding_Output, true, false
            },
            {
                    UA_TYPENAME("CH1_voltage")
                    UA_TYPES_FLOAT, HMP_padding_CH1_voltage, true,false
            },
            {
                    UA_TYPENAME("CH2_voltage")
                    UA_TYPES_FLOAT, HMP_padding_CH2_voltage, true, false
            },
            {
                    UA_TYPENAME("CH1_current")
                    UA_TYPES_FLOAT, HMP_padding_CH1_current, true, false
            },
            {
                    UA_TYPENAME("CH2_current")
                    UA_TYPES_FLOAT, HMP_padding_CH2_current, true, false
            }
    };


    const UA_DataType Type = {
            UA_TYPENAME("HMPMeasurements")             /* .typeName */
            {1, UA_NODEIDTYPE_NUMERIC, {4242}}, /* .typeId */
            sizeof(HMPMeasurements),                   /* .memSize */
            0,                               /* .typeIndex, in the array of custom t*/
            UA_DATATYPEKIND_STRUCTURE,       /* .typeKind */
            true,                            /* .pointerFree */
            false,                           /* .overlayable (depends on endianness
                                             the absence of padding) */
            7,                               /* .membersSize */
            0,                               /* .binaryEncodingId, the numeric
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
            char* hmpname=const_cast<char*>("HMPMeasurements");
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(language, hmpname);
            dattr.displayName = UA_LOCALIZEDTEXT(language, hmpname);
            dattr.dataType = Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;
            HMPMeasurements hmp=get();
            UA_Variant_setScalar(&dattr.value, &hmp, &Type);
            UA_QualifiedName TypeQName=UA_QUALIFIEDNAME_ALLOC(1, "HMPMeasurements");
            UA_Server_addVariableTypeNode(server, Type.typeId,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                          TypeQName,
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          dattr, nullptr, nullptr);
            UA_QualifiedName_deleteMembers(&TypeQName);

        }



    HMPMeasurements get(bool ch1, bool ch2, bool output,
                                              std::string  ch1_v,std::string  ch2_v,std::string ch1_c,std::string ch2_c){
        HMPMeasurements hmp;
        hmp.CH1 = ch1;
        hmp.CH2 = ch2;
        hmp.Output = output;

        hmp.CH1_voltage = std::stof(ch1_v);
        hmp.CH2_voltage = std::stof(ch2_v);
        hmp.CH1_current = std::stof(ch1_c);
        hmp.CH2_current = std::stof(ch2_c);

        return hmp;}

    HMPMeasurements get(){
        std::string zero="0";
        HMPMeasurements hmp=get(false,false,true,zero,zero,zero,zero);
    return hmp;}


};

static std::ostream& operator<<(std::ostream& os, const HMPMeasurements& hmp){
os<<hmp.CH1<<", "<<hmp.CH2<<", "<<hmp.Output<<", "<<hmp.CH1_voltage<<", "<<hmp.CH2_voltage<<", "<<hmp.CH1_current<<", "<<hmp.CH2_current;
return os;
}

#endif // HMPMeasurementsCONTAINERS_H
