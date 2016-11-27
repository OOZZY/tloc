#!/usr/bin/env bash

CFLAGS="-std=c11 -pedantic -Wall -Wextra -Werror -g -I. tlo/*.c"
CLANG_FLAGS="$CFLAGS -Weverything"
VALGRIND_FLAGS="--tool=memcheck --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --error-exitcode=1"

set -xe

clang ${CLANG_FLAGS} *.c -o tloc_test
gcc ${CFLAGS} *.c -o tloc_test-gcc

valgrind ${VALGRIND_FLAGS} ./tloc_test
valgrind ${VALGRIND_FLAGS} ./tloc_test-gcc

echo "All tests passed."
