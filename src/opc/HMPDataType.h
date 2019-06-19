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

  //  hmp.CH1_voltage = std::stod(ch1_v);
  //  hmp.CH2_voltage = std::stod(ch2_v);
  //  hmp.CH1_current = std::stod(ch1_c);
  //  hmp.CH2_current = std::stod(ch2_c);

    /*
    hmp.CH1_voltage = UA_String_fromChars(ch1_v.c_str());
    hmp.CH2_voltage = UA_String_fromChars(ch2_v.c_str());
    hmp.CH1_current = UA_String_fromChars(ch1_c.c_str());
    hmp.CH2_current = UA_String_fromChars(ch2_c.c_str());
*/
    char * writable1 = (char*)malloc(ch1_v.length()+1);
    std::copy(ch1_v.begin(), ch1_v.end(), writable1);
   writable1[ch1_v.size()] = '\0';
    char * writable2 = (char*)malloc(ch2_v.length()+1);
    std::copy(ch2_v.begin(), ch2_v.end(), writable2);
    writable2[ch2_v.size()] = '\0';
    char * writable3 = (char*)malloc(ch1_c.length()+1);
    std::copy(ch1_c.begin(), ch1_c.end(), writable3);
    writable3[ch1_c.size()] = '\0';
    char * writable4 = (char*)malloc(ch2_c.length()+1);
    std::copy(ch2_c.begin(), ch2_c.end(), writable4);
    writable4[ch2_c.size()] = '\0';

    /*
    char* writable1 = strcpy((char*)malloc(ch1_v.length()+1), ch1_v.c_str());
    char* writable2 = strcpy((char*)malloc(ch2_v.length()+1), ch2_v.c_str());
    char* writable3 = strcpy((char*)malloc(ch1_c.length()+1), ch1_c.c_str());
    char* writable4 = strcpy((char*)malloc(ch2_c.length()+1), ch2_c.c_str());
*/


     hmp.CH1_voltage = UA_STRING(writable1);
    hmp.CH2_voltage = UA_STRING(writable2);
    hmp.CH1_current = UA_STRING(writable3);
    hmp.CH2_current = UA_STRING(writable4);

    return hmp;}
static void delete_HMPMeasurementes(HMPMeasurements* hmp){
   UA_String_deleteMembers(&(hmp->CH1_voltage));
    UA_String_deleteMembers(&(hmp->CH2_voltage));
    UA_String_deleteMembers(&(hmp->CH1_current));
    UA_String_deleteMembers(&(hmp->CH2_current));
}

static HMPMeasurements new_HMPMeasurements(){
    //char* zero=const_cast<char*>("12");
    std::string zero="12";
    HMPMeasurements hmp=new_HMPMeasurements(true,false,false,zero,zero,zero,zero);
return hmp;}

#endif // HMPMeasurementsCONTAINERS_H
