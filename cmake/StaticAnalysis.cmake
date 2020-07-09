if(ANALYSE)
  option(USE_CLANGTIDY OFF)
  if(USE_CLANGTIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
      set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY}
                               -extra-arg=-Wno-unknown-warning-option)
      message(STATUS "Static analysis with:\t\t\tCLANG-TIDY")
    else()
      message(STATUS "Clang-Tidy not found. Run apt-get install llvm")
    endif()
  endif()

  option(USE_CPPCHECK ON)
  if(USE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
      set(CMAKE_CXX_CPPCHECK
          ${CPPCHECK}
          --suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck-suppressions.txt
          --enable=all
          --inline-suppr
          --inconclusive
          -i${CMAKE_SOURCE_DIR}/build
          -i${CMAKE_SOURCE_DIR}/deps)

      message(STATUS "Static analysis with:\t\t\tCPPCHECK")
    else()
      message(STATUS "Cppcheck not found. Run apt-get install cppcheck")
    endif()
  endif()
endif()
