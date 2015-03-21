#!/usr/bin/env bash

# ./test.sh; echo $?

CFLAGS="-std=c11 -pedantic -Wall -Wextra -Werror -g -I. tlo/*.c"
VALGRIND_FLAGS="--tool=memcheck --leak-check=yes --track-origins=yes"

set -xe

clang ${CFLAGS} darray_test.c -o darray_test
gcc ${CFLAGS} darray_test.c -o darray_test-gcc
clang ${CFLAGS} sllist_test.c -o sllist_test
gcc ${CFLAGS} sllist_test.c -o sllist_test-gcc

valgrind ${VALGRIND_FLAGS} ./darray_test
valgrind ${VALGRIND_FLAGS} ./sllist_test
