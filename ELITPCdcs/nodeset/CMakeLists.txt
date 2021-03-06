set(SUBMODULE_NAME "nodeset")
message(STATUS "Adding CMake fragment for submodule:\t\t"
               ${MODULE_NAME}_${SUBMODULE_NAME})
find_package(open62541 REQUIRED COMPONENTS FullNamespace)

if(NOT UA_TOOLS_DIR)
  set(UA_TOOLS_DIR ${open62541_TOOLS_DIR})
endif()

if(NOT UA_NODESET_DIR)
  set(UA_NODESET_DIR ${open62541_NODESET_DIR})
endif()

set(FILE_CSV_DIRPREFIX ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME})
set(FILE_BSD_DIRPREFIX ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME})
set(FILE_NS_DIRPREFIX ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME})

ua_generate_nodeset_and_datatypes(
  NAME
  "elitpcnodeset"
  FILE_CSV
  "${FILE_CSV_DIRPREFIX}/nodeset.csv"
  FILE_BSD
  "${FILE_BSD_DIRPREFIX}/nodeset.bsd"
  NAMESPACE_IDX
  3
  FILE_NS
  "${FILE_NS_DIRPREFIX}/nodeset.xml"
  INTERNAL)
add_library(
  ${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${UA_NODESET_ELITPCNODESET_SOURCES}
                                          ${UA_TYPES_ELITPCNODESET_SOURCES})
target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME} open62541::open62541)

install(TARGETS ${MODULE_NAME}_${SUBMODULE_NAME} LIBRARY DESTINATION lib)
