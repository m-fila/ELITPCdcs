function(add_clang_format_target)
  if(NOT ${PROJECT_NAME}_CLANG_FORMAT_BINARY)
    find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
  endif()
  if(${PROJECT_NAME}_CLANG_FORMAT_BINARY)
    add_custom_target(
      clang-format
      COMMAND
        ${${PROJECT_NAME}_CLANG_FORMAT_BINARY} -i
        ${CMAKE_CURRENT_LIST_DIR}/${sources}
        ${CMAKE_CURRENT_LIST_DIR}/${headers})
    # message( "Format the project using the `clang-format` target (i.e: cmake
    # --build build --target clang-format).\n" )
  endif()
endfunction()
