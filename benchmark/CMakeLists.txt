if (TLOC_COMPILE_FOR_GCOV AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(gcov_link_options gcov)
endif()

add_executable(tloc_list_benchmark tloc_list_benchmark.c)
set_target_properties(tloc_list_benchmark PROPERTIES C_EXTENSIONS OFF)
target_compile_options(tloc_list_benchmark PRIVATE ${global_compile_options})
target_compile_definitions(tloc_list_benchmark
  PRIVATE ${global_compile_definitions})
target_include_directories(tloc_list_benchmark
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tloc_list_benchmark PRIVATE tloc ${gcov_link_options})