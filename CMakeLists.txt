cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project("libtloc")
enable_testing()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-std=c11 -pedantic -Wall -Wextra -Werror")
endif()

if (NOT CONFIGURED_ONCE)
  set(CMAKE_C_FLAGS "${warnings}" CACHE STRING
      "Flags used by the compiler during all build types." FORCE)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(tlo)
add_executable(libtloc_test darray_test.c sllist_test.c libtloc_test.c)
target_link_libraries(libtloc_test tloc)
add_test(libtloc_test libtloc_test)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
