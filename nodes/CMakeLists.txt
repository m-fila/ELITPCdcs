cmake_minimum_required(VERSION 3.0...3.12)
project(open62541-examples)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
# This examples folder can also be built standalone.
# First install open62541 using `make install` then
# copy this folder to any other location and call CMake directly:
#
# cp -r open62541/examples $HOME/open62541_examples
# cd $HOME/open62541_examples
# mkdir build && cd build
# cmake -DUA_NAMESPACE_ZERO=FULL ..
# make -j

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

    function(assign_source_group)
        # define empty function. We don't need it in standalone
    endfunction(assign_source_group)

    include_directories(${PROJECT_BINARY_DIR}/src_generated)
endif()

# Required for common.h header file used in examples
include_directories(${CMAKE_CURRENT_LIST_DIR})


macro(add_example EXAMPLE_NAME EXAMPLE_SOURCE)
  add_executable(${EXAMPLE_NAME} ${STATIC_OBJECTS} ${EXAMPLE_SOURCE} ${ARGN})
  target_link_libraries(${EXAMPLE_NAME} open62541::open62541)
  assign_source_group(${EXAMPLE_SOURCE})
  set_target_properties(${EXAMPLE_NAME} PROPERTIES FOLDER "open62541/examples")
  set_target_properties(${EXAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  if(UA_COMPILE_AS_CXX)
      set_source_files_properties(${EXAMPLE_SOURCE} PROPERTIES LANGUAGE CXX)
  endif()
endmacro()

set(FILE_CSV_DIRPREFIX ${PROJECT_SOURCE_DIR})
set(FILE_BSD_DIRPREFIX ${PROJECT_SOURCE_DIR})
set(FILE_NS_DIRPREFIX ${PROJECT_SOURCE_DIR})

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # needed or cmake doesn't recognize dependencies of generated files
    set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR})
endif()


#if(UA_NAMESPACE_ZERO STREQUAL "FULL")
    ua_generate_nodeset_and_datatypes(
        NAME "testnodeset"
        FILE_CSV "${FILE_CSV_DIRPREFIX}/my.csv"
        FILE_BSD "${FILE_BSD_DIRPREFIX}/my.bsd"
        NAMESPACE_IDX 2
        FILE_NS "${FILE_NS_DIRPREFIX}/my.xml"
        INTERNAL
    )
    add_example(server_testnodeset server_testnodeset.c
                ${UA_NODESET_TESTNODESET_SOURCES}
                ${UA_TYPES_TESTNODESET_SOURCES})
    add_dependencies(server_testnodeset open62541-generator-ns-testnodeset)
#endif()

