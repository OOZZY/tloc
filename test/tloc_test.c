#include <stdio.h>
#include <tlo/stopwatch.h>
#include <tlo/test.h>
#include "cdarray_test.h"
#include "darray_test.h"
#include "dllist_test.h"
#include "list_test_utils.h"
#include "sllist_test.h"
#include "statistics_test.h"

static void testList(void) {
  testListDeleteWithNull();
  puts("================");
  puts("List tests done.");
  puts("================");
}

int main(void) {
  TloStopwatch stopwatch;

  tloStopwatchStart(&stopwatch);
  testList();
  testDArray();
  testSLList();
  testCDArray();
  testDLList();
  testStatistics();
  tloStopwatchStop(&stopwatch);

  puts("===============");
  puts("All tests done.");
  printf("Time taken: %Lg seconds (%Lg ticks)\n",
         tloStopwatchNumSeconds(&stopwatch),
         (long double)(tloStopwatchNumTicks(&stopwatch)));
  puts("===============");

  tloTestPrintReport();
  tloTestExit();
}
