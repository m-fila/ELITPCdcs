include_directories(${PROJECT_SOURCE_DIR}/COREdcs/server/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)
include_directories(${PROJECT_SOURCE_DIR}/test)
include_directories(${PROJECT_SOURCE_DIR}/ELITPCdcs/server/include)

add_executable(test_protocol protocol.cpp)

target_link_libraries(test_protocol COREdcs_server ELITPCdcs_server)



