
project(dcs_server)
find_package(open62541 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_BINARY_DIR}/src_generated)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)

add_executable(dcsServer ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES})
target_link_libraries(dcsServer open62541::open62541 dcsNodes common)

