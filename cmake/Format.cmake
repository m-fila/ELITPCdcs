add_custom_target(format)
include(FindPackageHandleStandardArgs)

find_program(
  CLANG_FORMAT_EXECUTABLE
  NAMES clang-format-10
        clang-format-8
        clang-format-3.9
        clang-format-3.8
        clang-format-3.7
        clang-format-3.6
        clang-format
  HINTS $ENV{CLANG_FORMAT_DIR}
  PATH_SUFFIXES bin
  DOC "Clang c/c++ files formatter")

find_package_handle_standard_args(clang-format DEFAULT_MSG
                                  CLANG_FORMAT_EXECUTABLE)

mark_as_advanced(CLANG_FORMAT_EXECUTABLE)

if(CLANG_FORMAT_EXECUTABLE)
  file(GLOB_RECURSE C_FILES_TO_FORMAT ${PROJECT_SOURCE_DIR}/COREdcs/*cpp
       ${PROJECT_SOURCE_DIR}/COREdcs/*h ${PROJECT_SOURCE_DIR}/ELITPCdcs/*cpp
       ${PROJECT_SOURCE_DIR}/ELITPCdcs/*h)
  add_custom_target(clang-format COMMAND ${CLANG_FORMAT_EXECUTABLE} -style=file
                                         -i ${C_FILES_TO_FORMAT})
  add_dependencies(format clang-format)
endif()

find_program(
  CMAKE_FORMAT_EXECUTABLE
  NAMES cmake-format
  HINTS $ENV{CMAKE_FORMAT_DIR}
  PATH_SUFFIXES bin
  DOC "CMake files formatter")

find_package_handle_standard_args(cmake-format DEFAULT_MSG
                                  CMAKE_FORMAT_EXECUTABLE)

mark_as_advanced(CMAKE_FORMAT_EXECUTABLE)

if(CMAKE_FORMAT_EXECUTABLE)
  file(GLOB_RECURSE CMAKE_FILES_TO_FORMAT ${CMAKE_SOURCE_DIR}/*CMakeLists.txt)

  add_custom_target(cmake-format COMMAND ${CMAKE_FORMAT_EXECUTABLE} -i
                                         ${CMAKE_FILES_TO_FORMAT})

  add_dependencies(format cmake-format)
endif()
