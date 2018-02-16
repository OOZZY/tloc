add_library(tloc STATIC util.c darray.c sllist.c test.c)
target_include_directories(tloc PUBLIC ${PROJECT_SOURCE_DIR}/include)
