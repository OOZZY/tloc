add_custom_target(tloc_test_headers SOURCES tloc_test.h)

add_executable(tloc_test darray_test.c sllist_test.c tloc_test.c)
target_include_directories(tloc_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if (MINGW)
  # make both gcc and clang of MSYS2 use the MinGW-w64 version of the printf
  # family which supports %zu
  target_compile_definitions(tloc_test PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif()
target_link_libraries(tloc_test PRIVATE tloc)

add_test(NAME tloc_test COMMAND tloc_test)
