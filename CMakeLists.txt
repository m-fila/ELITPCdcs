cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(
  dcs
  VERSION 1.8.3
  LANGUAGES CXX C)
message("\nStarted CMake for ${PROJECT_NAME} v${PROJECT_VERSION}")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(Utils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -g3 -fno-inline -Wall -Wno-unused-function")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "/opt/soft/${PROJECT_NAME}-v${PROJECT_VERSION}-install"
      CACHE PATH "..." FORCE)
endif()
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(open62541_LIB $ENV{open62541_LIB})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib ${open62541_LIB})

message(STATUS "CMAKE_INSTALL_PREFIX:\t\t" ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_INSTALL_RPATH:\t\t" ${CMAKE_INSTALL_RPATH})
find_package(open62541 REQUIRED)
if(NOT open62541_LIB)
  message(WARNING "Missing parts of open62541. Try '. env.sh'")
endif()

add_subdirectory(docs)

option(ANALYSE "do static analysis" OFF)
include(StaticAnalysis)

option(BUILD_SERVER "build server" ON)
option(BUILD_GUI "build operational client with gui" ON)
option(BUILD_CHECKS "build service checks" ON)
option(BUILD_TEST "build simple tests" OFF)

add_subdirectory(deps)
add_subdirectory(COREdcs)
add_subdirectory(ELITPCdcs)

configure_file(${CMAKE_SOURCE_DIR}/cmake/ProjectVersion.h.in
               ${CMAKE_SOURCE_DIR}/deps/include/ProjectVersion.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/dcsDbDump.py.in
               ${CMAKE_BINARY_DIR}/bin/dcsDbDump @ONLY)
install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/dcsDbDump DESTINATION bin)
configure_file(${CMAKE_SOURCE_DIR}/tools/thisdcs.sh.in
               ${CMAKE_BINARY_DIR}/bin/thisdcs.sh @ONLY)
install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/thisdcs.sh DESTINATION bin)
option(BUILD_TEST "build simple tests" OFF)
if(BUILD_TEST)
  enable_testing()
  # include(GoogleTest)
  add_subdirectory(test)
endif()
install(FILES $<1:${PROJECT_SOURCE_DIR}/docs/icons> DESTINATION icons)
include(Format)
add_subdirectory(package)
