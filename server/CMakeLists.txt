
project(dcs_server)
find_package(open62541 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common)
include_directories(${CMAKE_BINARY_DIR}/src_generated)
file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*/*.cpp)



add_executable(dcsServer ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${COMMON_SOURCES}
#${UA_NODESET_DCSNODESET_SOURCES} ${UA_TYPES_DCSNODESET_SOURCES}
)
target_link_libraries(dcsServer open62541::open62541 dcsNodes)
#add_dependencies(dcsServer open62541-generator-ns-dcsnodeset)
#target_link_libraries( dcsServer dcsNodes )
