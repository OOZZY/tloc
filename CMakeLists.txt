cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("tloc")
enable_testing()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(warning_compile_options -pedantic -Wall -Wextra -Werror)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(warning_compile_options ${warning_compile_options} -Weverything)
  endif()
endif()

add_subdirectory(src)
add_subdirectory(test)
