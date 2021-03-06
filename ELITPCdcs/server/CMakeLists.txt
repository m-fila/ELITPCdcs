set(SUBMODULE_NAME "server")
message(STATUS "Adding CMake fragment for submodule:\t\t"
               ${MODULE_NAME}_${SUBMODULE_NAME})

file(GLOB_RECURSE sources
     ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/src/*.cpp)

include_directories(
  ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/COREdcs/${SUBMODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/common)
include_directories(${PROJECT_SOURCE_DIR}/COREdcs/common)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)

add_library(${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${sources})
add_executable(dcsServer bin/main.cpp)

target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME}
                      COREdcs_${SUBMODULE_NAME} ${MODULE_NAME}_nodeset)
target_link_libraries(dcsServer ${MODULE_NAME}_${SUBMODULE_NAME})

install(TARGETS ${MODULE_NAME}_${SUBMODULE_NAME} LIBRARY DESTINATION lib)
install(TARGETS dcsServer RUNTIME DESTINATION bin)
