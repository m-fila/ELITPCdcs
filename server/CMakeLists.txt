cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)
project(dcs_server)

find_package(open62541 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common)
file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

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

add_executable(server ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${COMMON_SOURCES}
${UA_NODESET_DCSNODESET_SOURCES} ${UA_TYPES_DCSNODESET_SOURCES}
)
target_link_libraries(server open62541::open62541)

    add_dependencies(server open62541-generator-ns-dcsnodeset)
