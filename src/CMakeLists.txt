macro(prepend listVar prefix)
   set(${listVar} "")
   foreach(item ${ARGN})
      list(APPEND ${listVar} ${prefix}${item})
   endforeach(item)
endmacro(prepend)

if (TLOC_USE_ASAN_UBSAN AND NOT TLOC_USE_MSAN_UBSAN)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
      "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(sanitizer_compile_options -fsanitize=address -fsanitize=undefined -O1
      -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    set(sanitizer_link_options -fsanitize=address -fsanitize=undefined)
  endif()
endif()

if (TLOC_USE_MSAN_UBSAN AND NOT TLOC_USE_ASAN_UBSAN)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(sanitizer_compile_options -fsanitize=memory -fsanitize=undefined -O1
      -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    set(sanitizer_link_options -fsanitize=memory -fsanitize=undefined)
  endif()
endif()

if (UNIX)
    set(math_link_options m)
endif()

set(tloc_public_headers benchmark.h cdarray.h darray.h debug.h dllist.h hash.h
  list.h map.h set.h sllist.h statistics.h stopwatch.h test.h util.h)
set(tloc_private_headers list.h map.h set.h util.h)
set(tloc_sources benchmark.c cdarray.c darray.c dllist.c hash.c list.c map.c
  set.c sllist.c statistics.c stopwatch.c test.c util.c)
prepend(tloc_public_headers ${PROJECT_SOURCE_DIR}/include/tlo/
  ${tloc_public_headers})
add_library(tloc STATIC ${tloc_public_headers} ${tloc_private_headers}
  ${tloc_sources})
set_target_properties(tloc PROPERTIES C_EXTENSIONS OFF)
target_compile_features(tloc PUBLIC c_std_11)
target_compile_options(tloc PRIVATE ${global_compile_options}
  PUBLIC ${sanitizer_compile_options})
target_compile_definitions(tloc PRIVATE ${global_compile_definitions})
target_include_directories(tloc PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tloc PUBLIC ${sanitizer_link_options}
  ${math_link_options})
