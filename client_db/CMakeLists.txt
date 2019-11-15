project(client_db)
find_package(open62541 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common)
include_directories(${CMAKE_BINARY_DIR}/src_generated)
file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(dcsDb ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES} ${COMMON_SOURCES}
)
target_link_libraries(dcsDb sqlite3 open62541::open62541 dcsNodes)

