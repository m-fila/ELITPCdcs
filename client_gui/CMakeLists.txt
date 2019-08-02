cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(client_gui)
set (CMAKE_CXX_STANDARD 11)
#find_package(open62541 REQUIRED)
find_package(Qt5Widgets)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common)
include_directories (${PROJECT_BINARY_DIR})
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Examples are built standalone. Find installed open62541

    if(UA_NAMESPACE_ZERO STREQUAL "FULL")
        find_package(open62541 REQUIRED COMPONENTS FullNamespace)
    else()
        find_package(open62541 REQUIRED)
    endif()

    if(NOT UA_TOOLS_DIR)
        set(UA_TOOLS_DIR ${open62541_TOOLS_DIR})
    endif()

    if(NOT UA_NODESET_DIR)
        set(UA_NODESET_DIR ${open62541_NODESET_DIR})
    endif()

    include_directories(${PROJECT_BINARY_DIR}/src_generated)
endif()

set(FILE_CSV_DIRPREFIX ${PROJECT_SOURCE_DIR}/../nodes)
set(FILE_BSD_DIRPREFIX ${PROJECT_SOURCE_DIR}/../nodes)
set(FILE_NS_DIRPREFIX ${PROJECT_SOURCE_DIR}/../nodes)

#if(UA_NAMESPACE_ZERO STREQUAL "FULL")
    ua_generate_nodeset_and_datatypes(
        NAME "dcsnodeset"
        FILE_CSV "${FILE_CSV_DIRPREFIX}/nodeset.csv"
        FILE_BSD "${FILE_BSD_DIRPREFIX}/nodeset.bsd"
        NAMESPACE_IDX 2
        FILE_NS "${FILE_NS_DIRPREFIX}/nodeset.xml"
        INTERNAL
    )




file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB FORMS ${PROJECT_SOURCE_DIR}/forms/*.ui)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB RES ${PROJECT_SOURCE_DIR}/*.qrc)
qt5_wrap_cpp(H_MOC ${HEADERS})
qt5_wrap_ui(UI_MOC ${FORMS})
add_library(CLIENT_LIB  ${SOURCES} ${H_MOC} ${UI_MOC})
target_link_libraries(CLIENT_LIB Qt5::Widgets)
add_executable(client_gui ${PROJECT_SOURCE_DIR}/main.cpp
${UA_NODESET_DCSNODESET_SOURCES} ${UA_TYPES_DCSNODESET_SOURCES}
 ${SOURCES} ${COMMON_SOURCES} ${RES}
)
target_link_libraries(client_gui  CLIENT_LIB open62541::open62541)
add_dependencies(client_gui open62541-generator-ns-dcsnodeset)
