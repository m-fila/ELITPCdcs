#ifndef HMPMeasurementsCONTAINERS_H
#define HMPMeasurementsCONTAINERS_H



#include <string>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

typedef struct{
    UA_Boolean CH1;
    UA_Boolean CH2;
    UA_Boolean Output;
    UA_String CH1_voltage;
    UA_String CH2_voltage;
    UA_String CH1_current;
    UA_String CH2_current;
    } HMPMeasurements;
#define HMP_padding_CH2 offsetof(HMPMeasurements,CH2) - offsetof(HMPMeasurements,CH1) - sizeof(UA_Boolean)
#define HMP_padding_Output offsetof(HMPMeasurements,Output) - offsetof(HMPMeasurements,CH2) - sizeof(UA_Boolean)
#define HMP_padding_CH1_voltage offsetof(HMPMeasurements,CH1_voltage) - offsetof(HMPMeasurements,Output) - sizeof(UA_Boolean)
#define HMP_padding_CH2_voltage offsetof(HMPMeasurements,CH2_voltage) - offsetof(HMPMeasurements,CH1_voltage) - sizeof(UA_String)
#define HMP_padding_CH1_current offsetof(HMPMeasurements,CH1_current) - offsetof(HMPMeasurements,CH2_voltage) - sizeof(UA_String)
#define HMP_padding_CH2_current offsetof(HMPMeasurements,CH2_current) - offsetof(HMPMeasurements,CH1_current) - sizeof(UA_String)

static UA_DataTypeMember HMP_members[7] = {
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
                UA_TYPES_STRING, HMP_padding_CH1_voltage, true,false
        },
        {
                UA_TYPENAME("CH2_voltage")
                UA_TYPES_STRING, HMP_padding_CH2_voltage, true, false
        },
        {
                UA_TYPENAME("CH1_current")
                UA_TYPES_STRING, HMP_padding_CH1_current, true, false
        },
        {
                UA_TYPENAME("CH2_current")
                UA_TYPES_STRING, HMP_padding_CH2_current, true, false  
        }
};


static const UA_DataType HMPType = {
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
        HMP_members
};



static HMPMeasurements new_HMPMeasurements(bool ch1, bool ch2, bool output,
                                          std::string  ch1_v,std::string  ch2_v,std::string ch1_c,std::string ch2_c){
    HMPMeasurements hmp;
    hmp.CH1 = ch1;
    hmp.CH2 = ch2;
    hmp.Output = output;
    /*
    hmp.CH1_voltage = UA_STRING(ch1_v);
    hmp.CH2_voltage = UA_STRING(ch2_v);
    hmp.CH1_current = UA_STRING(ch1_c);
    hmp.CH2_current = UA_STRING(ch2_c);
    */
    char * writable1 = new char[ch1_v.size() + 1];
    std::copy(ch1_v.begin(), ch1_v.end(), writable1);
    writable1[ch1_v.size()] = '\0';
    char * writable2 = new char[ch2_v.size() + 1];
    std::copy(ch2_v.begin(), ch2_v.end(), writable2);
    writable2[ch2_v.size()] = '\0';
    char * writable3 = new char[ch1_c.size() + 1];
    std::copy(ch1_c.begin(), ch1_c.end(), writable3);
    writable3[ch1_c.size()] = '\0';
    char * writable4 = new char[ch2_c.size() + 1];
    std::copy(ch2_c.begin(), ch2_c.end(), writable4);
    writable4[ch2_c.size()] = '\0';
    hmp.CH1_voltage = UA_STRING(writable1);
    hmp.CH2_voltage = UA_STRING(writable2);
    hmp.CH1_current = UA_STRING(writable3);
    hmp.CH2_current = UA_STRING(writable4);

    return hmp;}

static HMPMeasurements new_HMPMeasurements(){
    //char* zero=const_cast<char*>("12");
    std::string zero="12";
    HMPMeasurements hmp=new_HMPMeasurements(true,false,false,zero,zero,zero,zero);
return hmp;}

static void addHMPMeasurementsDataType(UA_Server *server, UA_ServerConfig *config) {
        UA_DataType *types = static_cast<UA_DataType*> (UA_malloc(sizeof(UA_DataType)));
        UA_DataTypeMember *members = static_cast<UA_DataTypeMember*>(UA_malloc(sizeof(UA_DataTypeMember) * 7));
        for(int i=0; i!=7;i++){
        members[i] = HMP_members[i];}
        types[0] = HMPType;
        types[0].members = members;

        UA_DataTypeArray customDataTypes = {config->customDataTypes, 1, types};
        config->customDataTypes = &customDataTypes;


        char* language=const_cast<char*>("en-Us");
        char* hmpname=const_cast<char*>("HMPMeasurements");
        UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
        dattr.description = UA_LOCALIZEDTEXT(language, hmpname);
        dattr.displayName = UA_LOCALIZEDTEXT(language, hmpname);
        dattr.dataType = HMPType.typeId;
        dattr.valueRank = UA_VALUERANK_SCALAR;
        HMPMeasurements hmp=new_HMPMeasurements();
        UA_Variant_setScalar(&dattr.value, &hmp, &HMPType);

        UA_Server_addVariableTypeNode(server, HMPType.typeId,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                      UA_QUALIFIEDNAME(1, const_cast<char*>("HMPMeasurements")),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                      dattr, nullptr, nullptr);
    }

/*
static char* HMPVariableName=const_cast<char*>("HMP.variable");
static void addHMPMeasurementsVariable(UA_Server *server) {
        HMPMeasurements hmp=new_HMPMeasurements();
        UA_VariableAttributes vattr = UA_VariableAttributes_default;
        char* language=const_cast<char*>("en-Us");
        char* variablename=const_cast<char*>("HMPvariable");
        vattr.description = UA_LOCALIZEDTEXT(language, variablename);
        vattr.displayName = UA_LOCALIZEDTEXT(language, variablename);
        vattr.dataType = HMPType.typeId;
        vattr.valueRank = UA_VALUERANK_SCALAR;
        UA_Variant_setScalar(&vattr.value, &hmp, &HMPType);

        UA_Server_addVariableNode(server, UA_NODEID_STRING(1, HMPVariableName),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                  UA_QUALIFIEDNAME(1, HMPVariableName),
                                  HMPType.typeId, vattr, nullptr, nullptr);
    }
*/


#endif // HMPMeasurementsCONTAINERS_H
