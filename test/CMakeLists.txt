if (TLOC_COMPILE_FOR_GCOV AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(gcov_link_options gcov)
endif()

set(tloc_test_headers cdarray_test.h darray_test.h dllist_test.h
  list_test_utils.h map_test_utils.h schtable_test.h set_test_utils.h
  sllist_test.h statistics_test.h util.h)
set(tloc_test_sources cdarray_test.c darray_test.c dllist_test.c
  list_test_utils.c map_test_utils.c schtable_test.c set_test_utils.c
  sllist_test.c statistics_test.c tloc_test.c util.c)
add_executable(tloc_test ${tloc_test_headers} ${tloc_test_sources})
set_target_properties(tloc_test PROPERTIES C_EXTENSIONS OFF)
target_compile_options(tloc_test PRIVATE ${global_compile_options})
target_compile_definitions(tloc_test PRIVATE ${global_compile_definitions})
target_include_directories(tloc_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tloc_test PRIVATE tloc ${gcov_link_options})

add_test(NAME tloc_test COMMAND tloc_test)
