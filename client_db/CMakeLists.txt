project(client_db)
find_package(open62541 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(dcsDb ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES})
target_link_libraries(dcsDb open62541::open62541 dcsNodes common ${CMAKE_DL_LIBS})

