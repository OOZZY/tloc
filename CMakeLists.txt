project("tloc")
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
enable_testing()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(flags "-std=c11 -pedantic -Wall -Wextra -Werror")
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(flags "${flags} -Weverything")
  endif()
  set(debug_flags "-g")
  #set(debug_flags "-g -fsanitize=address -fsanitize=undefined -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
  #set(debug_flags "-g -fsanitize=memory -fsanitize=undefined -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
else()
  set(flags "${CMAKE_C_FLAGS}")
  set(debug_flags "${CMAKE_C_FLAGS_DEBUG}")
endif()

message(STATUS "tloc setting flags")
set(CMAKE_C_FLAGS "${flags}" CACHE STRING
    "Flags used by the compiler during all build types." FORCE)
set(CMAKE_C_FLAGS_DEBUG "${debug_flags}" CACHE STRING
    "Flags used by the compiler during debug builds." FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(tlo)
add_executable(tloc_test darray_test.c sllist_test.c tloc_test.c)
target_link_libraries(tloc_test tloc)
add_test(tloc_test tloc_test)
