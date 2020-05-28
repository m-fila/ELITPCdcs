#include "DCSUAJson.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
#include <iostream>


int main() {

  UA_HMPc a;
  UA_HMPc_init(&a);
  UA_HMPm z;
  UA_HMPm_init(&z);
  int b[3] = {1, 2, 3};
  UA_Double *t =
      static_cast<UA_Double *>(UA_Array_new(7, &UA_TYPES[UA_TYPES_DOUBLE]));
  std::cout << UAJson::toStringViaVariant(&z,
                                &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPM])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(&a, &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(b, 3, &UA_TYPES[UA_TYPES_INT32])
            << std::endl;
  std::cout << UAJson::toStringViaVariant(t, 7, &UA_TYPES[UA_TYPES_DOUBLE])
            << std::endl;

  return 0;
}