set(MODULE_NAME "test")

message(STATUS "Adding CMake fragment for module:\t\t" ${MODULE_NAME})

add_subdirectory(${PROJECT_SOURCE_DIR}/test/googletest)

mark_as_advanced(
  BUILD_GMOCK
  BUILD_GTEST
  BUILD_SHARED_LIBS
  INSTALL_GTEST
  gmock_build_tests
  gtest_build_samples
  gtest_build_tests
  gtest_disable_pthreads
  gtest_force_shared_crt
  gtest_hide_internal_symbols)
set_target_properties(gtest PROPERTIES FOLDER deps)
set_target_properties(gtest_main PROPERTIES FOLDER deps)
set_target_properties(gmock PROPERTIES FOLDER deps)
set_target_properties(gmock_main PROPERTIES FOLDER deps)

include_directories(${PROJECT_SOURCE_DIR}/COREdcs/server/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/include)
include_directories(${CMAKE_BINARY_DIR}/src_generated)
include_directories(${PROJECT_SOURCE_DIR}/ELITPCdcs/server/include)
include_directories(${PROJECT_SOURCE_DIR}/ELITPCdcs/shared)

macro(add_server_test NAME)
  add_executable(${NAME} ${NAME}.cpp)
  target_link_libraries(${NAME} gmock gtest gtest_main COREdcs_server
                        ELITPCdcs_server)
  add_test(NAME ${NAME} COMMAND ${NAME})
endmacro()

if(BUILD_SERVER)
  add_server_test(DCSPiWeatherTest)
  add_server_test(DCSServerTest)
  add_server_test(DCSDT1470etTest)
endif()
