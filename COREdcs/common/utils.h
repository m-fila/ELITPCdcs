#ifndef DCS_UTILS_H
#define DCS_UTILS_H
#include <open62541/types_generated.h>
#include <string>
namespace DCSUtils {
std::string UaToStd(UA_String string) {
    std::string str;
    if(string.length != 0) {
        str = std::string(reinterpret_cast<char *>(string.data), string.length);
    }
    return str;
}
};  // namespace DCSUtils

#endif  // DCS_UTILS_H
