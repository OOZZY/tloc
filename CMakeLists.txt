cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("tloc")
enable_testing()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options -pedantic -Wall -Wextra -Werror)
endif()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(global_compile_options ${global_compile_options} -Weverything -Wno-padded)
endif()

if (MINGW)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
      "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # make both gcc and clang of MSYS2 use the MinGW-w64 version of the printf
    # family which supports %zu
    set(global_compile_definitions __USE_MINGW_ANSI_STDIO=1)
  endif()

  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # with the above compile definition, printf will be marked unused for some
    # reason, so if printf is used, the compiler will emit the following
    # warning: 'printf' was marked unused but was used
    set(global_compile_options ${global_compile_options}
      -Wno-used-but-marked-unused)
  endif()
endif()

option(TLOC_USE_ASAN_UBSAN
  "Tell the compiler to use ASan and UBSan (GNU/Clang only)." OFF)
option(TLOC_USE_MSAN_UBSAN
  "Tell the compiler to use MSan and UBSan (Clang only)." OFF)

option(TLOC_COLORED_DIAGNOSTICS
  "Tell the compiler to use colors in diagnostics (GNU/Clang only)." ON)
if (TLOC_COLORED_DIAGNOSTICS)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(global_compile_options ${global_compile_options} -fdiagnostics-color)
  elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(global_compile_options ${global_compile_options} -fcolor-diagnostics)
  endif()
endif()

option(TLOC_COMPILE_FOR_GCOV
  "Tell the compiler to compile for gcov coverage analysis (GNU only)." OFF)
if (TLOC_COMPILE_FOR_GCOV AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(global_compile_options ${global_compile_options} -fprofile-arcs
    -ftest-coverage)
endif()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)
