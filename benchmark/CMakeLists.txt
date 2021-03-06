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

add_library(hash_benchmark_utils STATIC hash_benchmark_utils.h
  hash_benchmark_utils.c)
set_target_properties(hash_benchmark_utils PROPERTIES C_EXTENSIONS OFF)
target_compile_options(hash_benchmark_utils PRIVATE ${global_compile_options})
target_compile_definitions(hash_benchmark_utils PRIVATE
  ${global_compile_definitions})
target_include_directories(hash_benchmark_utils PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hash_benchmark_utils PRIVATE tloc)

add_executable(tloc_hash_consecutive_integers_benchmark
  tloc_hash_consecutive_integers_benchmark.c)
set_target_properties(tloc_hash_consecutive_integers_benchmark
  PROPERTIES C_EXTENSIONS OFF)
target_compile_options(tloc_hash_consecutive_integers_benchmark
  PRIVATE ${global_compile_options})
target_compile_definitions(tloc_hash_consecutive_integers_benchmark
  PRIVATE ${global_compile_definitions})
target_include_directories(tloc_hash_consecutive_integers_benchmark
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tloc_hash_consecutive_integers_benchmark
  PRIVATE tloc ${gcov_link_options} hash_benchmark_utils)

add_executable(tloc_hash_lines_benchmark tloc_hash_lines_benchmark.c)
set_target_properties(tloc_hash_lines_benchmark PROPERTIES C_EXTENSIONS OFF)
target_compile_options(tloc_hash_lines_benchmark
  PRIVATE ${global_compile_options})
target_compile_definitions(tloc_hash_lines_benchmark
  PRIVATE ${global_compile_definitions})
target_include_directories(tloc_hash_lines_benchmark
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tloc_hash_lines_benchmark
  PRIVATE tloc ${gcov_link_options} hash_benchmark_utils)
