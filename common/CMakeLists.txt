project(common)
include_directories(${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)

#add_executable(c ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES})



add_library(common  ${SOURCES})