#ifndef DCS_UA_JSON_H
#define DCS_UA_JSON_H

#include "ua_types_encoding_json.h"
#include <open62541/types_generated.h>
#include <open62541/types_generated_handling.h>
#include <string>

namespace DCSUAJson {
// Wrapper for UA_encode_Json. Writes only first index of an array
template <class T> std::string toString(T *data, const UA_DataType *type) {
    auto size = UA_calcSizeJson(data, type, NULL, 0, NULL, 0, UA_TRUE);
    UA_ByteString buf;
    UA_ByteString_allocBuffer(&buf, size + 1);
    UA_Byte *bufPos = &buf.data[0];
    const UA_Byte *bufEnd = &buf.data[size + 1];
    status s = UA_encodeJson(data, type, &bufPos, &bufEnd, NULL, 0, NULL, 0, UA_TRUE);
    std::string string;
    if(s == UA_STATUSCODE_GOOD) {
        *bufPos = 0;
        string = reinterpret_cast<char *>(buf.data);
    }
    UA_ByteString_deleteMembers(&buf);
    return string;
}

// Wrapper for UA_encode_Json. Hides value inside scalar Variant
template <class T> std::string toStringViaVariant(T *data, const UA_DataType *type) {
    UA_Variant var;
    UA_Variant_setScalar(&var, data, type);
    return toString(&var, &UA_TYPES[UA_TYPES_VARIANT]);
}
// Wrapper for UA_encode_Json. Hides value inside array Variant
template <class T>
std::string toStringViaVariant(T *data, size_t size, const UA_DataType *type) {
    UA_Variant var;
    UA_Variant_setArray(&var, data, size, type);
    return toString(&var, &UA_TYPES[UA_TYPES_VARIANT]);
}
}  // namespace DCSUAJson
#endif  // DCS_UA_JSON_H
