#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
#include "string"
#include "ua_types_encoding_json.h"
#include <iostream>
#include <open62541/types_generated.h>
#include <open62541/types_generated_handling.h>

namespace UAJson {
// Wrapper for UA_encode_Json. Writes only first index of an array
template <class T> std::string toString(T *data, const UA_DataType *type) {
  auto size = UA_calcSizeJson(data, type, NULL, 0, NULL, 0, UA_TRUE);
  UA_ByteString buf;
  UA_ByteString_allocBuffer(&buf, size + 1);
  UA_Byte *bufPos = &buf.data[0];
  const UA_Byte *bufEnd = &buf.data[size + 1];
  status s =
      UA_encodeJson(data, type, &bufPos, &bufEnd, NULL, 0, NULL, 0, UA_TRUE);
  std::string string;
  if (s == UA_STATUSCODE_GOOD) {
    *bufPos = 0;
    string = reinterpret_cast<char *>(buf.data);
  }
  UA_ByteString_deleteMembers(&buf);
  return string;
}
// Wrapper for UA_encode_Json. Hides value inside scalar Variant
template <class T>
std::string toStringViaVariant(T *data, const UA_DataType *type) {
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
} // namespace UAJson
int main() {

  UA_HMPc a;
  UA_HMPc_init(&a);
  int b[3] = {1, 2, 3};
  UA_Double *t =
      static_cast<UA_Double *>(UA_Array_new(7, &UA_TYPES[UA_TYPES_DOUBLE]));
  std::cout << UAJson::toString(&a,
                                &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(&a, &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(b, 3, &UA_TYPES[UA_TYPES_INT32])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(t, 7, &UA_TYPES[UA_TYPES_DOUBLE])
            << std::endl;

  return 0;
}