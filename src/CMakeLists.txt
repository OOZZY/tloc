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

set(tloc_headers util.h list.h darray.h sllist.h cdarray.h test.h debug.h)
set(tloc_sources util.c list.c darray.c sllist.c cdarray.c test.c)
prepend(tloc_headers ${PROJECT_SOURCE_DIR}/include/tlo/ ${tloc_headers})
add_library(tloc STATIC ${tloc_headers} ${tloc_sources})
set_target_properties(tloc PROPERTIES C_EXTENSIONS OFF)
target_compile_features(tloc PUBLIC c_std_11)
target_compile_options(tloc PRIVATE ${global_compile_options}
  PUBLIC ${sanitizer_compile_options})
target_include_directories(tloc PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tloc PUBLIC ${sanitizer_link_options})
