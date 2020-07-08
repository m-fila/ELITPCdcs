if(BUILD_DOCS)
  set(MODULE "docs")
  message(STATUS "Adding CMake fragment for module:\t\t" ${MODULE})

  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    set(DOXYGEN_PROJECT_NAME "ELITPC dcs")
    set(DOXYGEN_PROJECT_VERSION ${CMAKE_PROJECT_VERSION})
    set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
    add_custom_target(
      docs ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)

  else()
    message(WARNING "Missing doxygen. Run apt-get install doxygen")

  endif()
  message(STATUS "Docs configured\t\t\t\tOK")
endif()
