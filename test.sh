#!/usr/bin/env bash

CFLAGS="-std=c11 -pedantic -Wall -Wextra -Werror -g -I. tlo/*.c"
CLANG_FLAGS="$CFLAGS -Weverything"
VALGRIND_FLAGS="--tool=memcheck --leak-check=full --track-origins=yes \
  --error-exitcode=1"

set -xe

clang ${CLANG_FLAGS} darray_test.c -o darray_test
gcc ${CFLAGS} darray_test.c -o darray_test-gcc
clang ${CLANG_FLAGS} sllist_test.c -o sllist_test
gcc ${CFLAGS} sllist_test.c -o sllist_test-gcc

valgrind ${VALGRIND_FLAGS} ./darray_test
valgrind ${VALGRIND_FLAGS} ./sllist_test

echo "All tests passed."
