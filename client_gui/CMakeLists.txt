project(client_gui)
find_package(open62541 REQUIRED)
find_package(Qt5Widgets)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/common/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

file(GLOB COMMON_SOURCES ../common/*.cpp)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB FORMS ${PROJECT_SOURCE_DIR}/forms/*.ui)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB RES ${PROJECT_SOURCE_DIR}/*.qrc)

qt5_wrap_cpp(H_MOC ${HEADERS})
qt5_wrap_ui(UI_MOC ${FORMS})
add_library(clientQt  ${SOURCES} ${H_MOC} ${UI_MOC})
target_link_libraries(clientQt Qt5::Widgets open62541::open62541 common)

add_executable(dcsGui ${PROJECT_SOURCE_DIR}/main.cpp
 ${SOURCES} ${RES})
target_link_libraries(dcsGui  clientQt dcsNodes )
