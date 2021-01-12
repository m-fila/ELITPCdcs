set(SUBMODULE_NAME "clientGUI")
message(STATUS "Adding CMake fragment for submodule:\t\t"
               ${MODULE_NAME}_${SUBMODULE_NAME})
find_package(Qt5Widgets)
file(GLOB_RECURSE sources
     ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/src/*.cpp)
file(GLOB_RECURSE headers
     ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include/*.h)

include_directories(
  ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)

qt5_wrap_cpp(H_MOC ${headers})
add_library(${MODULE_NAME}_${SUBMODULE_NAME}_Qt SHARED ${SOURCES} ${H_MOC}
                                                       ${UI_MOC})
add_library(${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${sources})

target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME}_Qt Qt5::Widgets
                      open62541::open62541)
target_link_libraries(
  ${MODULE_NAME}_${SUBMODULE_NAME} open62541::open62541 ${MODULE_NAME}_nodeset
  Qt5::Widgets ${MODULE_NAME}_${SUBMODULE_NAME}_Qt)

install(TARGETS ${MODULE_NAME}_${SUBMODULE_NAME}_Qt LIBRARY DESTINATION lib)
install(TARGETS ${MODULE_NAME}_${SUBMODULE_NAME} LIBRARY DESTINATION lib)
