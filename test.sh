#!/usr/bin/env bash

# ./test.sh; echo $?

set -xe

clang -std=c11 -pedantic -Wall -Wextra -Werror -g -I. darray_test.c tlo/*.c -o darray_test
gcc -std=c11 -pedantic -Wall -Wextra -Werror -g -I. darray_test.c tlo/*.c -o darray_test-gcc

valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./darray_test
