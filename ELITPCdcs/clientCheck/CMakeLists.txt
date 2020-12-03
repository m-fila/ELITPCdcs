set(SUBMODULE_NAME "clientCheck")
message(STATUS "Adding CMake fragment for submodule:\t\t"
               ${MODULE_NAME}_${SUBMODULE_NAME})

# file(GLOB_RECURSE sources
# ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/src/*.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/shared)
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/server/include)
include_directories(${PROJECT_SOURCE_DIR}/COREdcs/server/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)

# add_library(${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${sources})
add_executable(dcsCheck bin/main.cpp)

# target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME}
# COREdcs_${SUBMODULE_NAME} ${MODULE_NAME}_nodeset)
target_link_libraries(dcsCheck ELITPCdcs_server open62541::open62541)

install(TARGETS dcsCheck RUNTIME DESTINATION bin)
