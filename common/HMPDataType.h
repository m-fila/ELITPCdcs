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

    hmp.CH1_voltage = std::stof(ch1_v);
    hmp.CH2_voltage = std::stof(ch2_v);
    hmp.CH1_current = std::stof(ch1_c);
    hmp.CH2_current = std::stof(ch2_c);

    return hmp;}

static HMPMeasurements new_HMPMeasurements(){
    std::string zero="0";
    HMPMeasurements hmp=new_HMPMeasurements(false,false,true,zero,zero,zero,zero);
return hmp;}

static std::ostream& operator<<(std::ostream& os, const HMPMeasurements& hmp){
os<<hmp.CH1<<" "<<hmp.CH2<<" "<<hmp.Output<<" "<<hmp.CH1_voltage<<" "<<hmp.CH1_voltage<<" "<<hmp.CH2_voltage<<" "<<hmp.CH2_current;
return os;
}

#endif // HMPMeasurementsCONTAINERS_H
