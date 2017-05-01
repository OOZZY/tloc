#include "tlo/test.h"
#include <stdio.h>
#include <stdlib.h>

static int numExpects = 0;
static int numExpectsFailed = 0;
static int numAsserts = 0;
static int numAssertsFailed = 0;

void tloTestExpect(bool isExpect, bool condition, const char *file, int line,
                   const char *func, const char *conditionString) {
  if (isExpect) {
    ++numExpects;
  } else {
    ++numAsserts;
  }

  if (!condition) {
    if (isExpect) {
      ++numExpectsFailed;
      printf("%s:%d: %s: expect failed: %s.\n", file, line, func,
             conditionString);
    } else {
      ++numAssertsFailed;
      printf("%s:%d: %s: assert failed: %s.\n", file, line, func,
             conditionString);
    }
  }
}

void tloTestPrintReport(void) {
  printf("=========================\n");
  printf("# expects       : %d\n", numExpects);
  printf("# expects failed: %d\n", numExpectsFailed);
  printf("# asserts       : %d\n", numAsserts);
  printf("# asserts failed: %d\n", numAssertsFailed);
  printf("=========================\n");
}

void tloTestExit(void) {
  if (numExpectsFailed || numAssertsFailed) {
    puts("[ FAILED  ]");
    exit(EXIT_FAILURE);
  }
  puts("[ PASSED  ]");
  exit(EXIT_SUCCESS);
}
