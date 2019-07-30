cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)
project(dcs_server)

find_package(open62541 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common)
file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)
add_executable(server ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${COMMON_SOURCES})
target_link_libraries(server open62541::open62541)