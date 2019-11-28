project(common)
include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(common  ${SOURCES})