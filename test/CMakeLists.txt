if (MINGW)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
      "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # make both gcc and clang of MSYS2 use the MinGW-w64 version of the printf
    # family which supports %zu
    set(mingw_compile_definitions __USE_MINGW_ANSI_STDIO=1)
  endif()

  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(mingw_compile_options -Wno-used-but-marked-unused)
  endif()
endif()

set(tloc_test_headers util.h list_test_utils.h tloc_test.h)
set(tloc_test_sources util.c list_test_utils.c list_test.c darray_test.c
  sllist_test.c tloc_test.c)
add_executable(tloc_test ${tloc_test_headers} ${tloc_test_sources})
set_target_properties(tloc_test PROPERTIES C_EXTENSIONS OFF)
target_compile_options(tloc_test
  PRIVATE ${global_compile_options} ${mingw_compile_options})
target_include_directories(tloc_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(tloc_test PRIVATE ${mingw_compile_definitions})
target_link_libraries(tloc_test PRIVATE tloc)

add_test(NAME tloc_test COMMAND tloc_test)
