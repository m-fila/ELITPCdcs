set(SUBMODULE_NAME "server")
MESSAGE( STATUS "Adding CMake fragment for submodule:\t\t" ${MODULE_NAME}_${SUBMODULE_NAME} )

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include)

add_library(${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${sources})

target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME} open62541::open62541 UA_JSON)

install(TARGETS ${MODULE_NAME}_${SUBMODULE_NAME} LIBRARY DESTINATION lib)