set(MODULE_NAME "deps")
message(STATUS "Adding CMake fragment for module:\t\t" ${MODULE_NAME})
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/include)
add_library(
  UA_JSON SHARED
  src/jsmn.cpp
  src/ua_types_encoding_json.c
  src/base64.c
  src/itoa.c
  src/atoi.c
  src/string_escape.c
  src/libc_time.c)
target_link_libraries(UA_JSON open62541::open62541)
install(TARGETS UA_JSON LIBRARY DESTINATION lib)
