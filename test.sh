#!/usr/bin/env bash

# ./test.sh; echo $?

set -xe

clang -std=c11 -pedantic -Wall -Wextra -Werror -g -I. darray_test.c tlo/*.c -o darray_test
gcc -std=c11 -pedantic -Wall -Wextra -Werror -g -I. darray_test.c tlo/*.c -o darray_test-gcc
clang -std=c11 -pedantic -Wall -Wextra -Werror -g -I. sllist_test.c tlo/*.c -o sllist_test
gcc -std=c11 -pedantic -Wall -Wextra -Werror -g -I. sllist_test.c tlo/*.c -o sllist_test-gcc

valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./darray_test
valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./sllist_test
