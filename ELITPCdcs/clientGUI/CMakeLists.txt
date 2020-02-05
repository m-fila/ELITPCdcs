set(SUBMODULE_NAME "clientGUI")
MESSAGE( STATUS "Adding CMake fragment for submodule:\t\t" ${MODULE_NAME}_${SUBMODULE_NAME} )
find_package(Qt5Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/src/*.cpp)
file(GLOB_RECURSE forms ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/forms/*.ui)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include/*.h)
file(GLOB_RECURSE res ${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/*.qrc)

include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/${SUBMODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/COREdcs/${SUBMODULE_NAME}/include)
include_directories(${PROJECT_SOURCE_DIR}/${MODULE_NAME}/shared)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)


qt5_wrap_cpp(H_MOC ${headers})
qt5_wrap_ui(UI_MOC ${forms})

add_library(${MODULE_NAME}_${SUBMODULE_NAME}_Qt SHARED ${SOURCES} ${H_MOC} ${UI_MOC})
add_library(${MODULE_NAME}_${SUBMODULE_NAME} SHARED ${sources} ${res})
add_executable(dcsGUI bin/main.cpp)

target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME}_Qt Qt5::Widgets open62541::open62541 )
target_link_libraries(${MODULE_NAME}_${SUBMODULE_NAME} COREdcs_${SUBMODULE_NAME} ${MODULE_NAME}_nodeset ${MODULE_NAME}_${SUBMODULE_NAME}_Qt)
target_link_libraries(dcsGUI ${MODULE_NAME}_${SUBMODULE_NAME})
