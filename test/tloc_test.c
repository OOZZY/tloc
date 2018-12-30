#include <stdio.h>
#include <tlo/stopwatch.h>
#include <tlo/test.h>
#include "cdarray_test.h"
#include "darray_test.h"
#include "dllist_test.h"
#include "list_test.h"
#include "sllist_test.h"

int main(void) {
  TloStopwatch stopwatch;

  tloStopwatchStart(&stopwatch);
  testList();
  testDArray();
  testSLList();
  testCDArray();
  testDLList();
  tloStopwatchStop(&stopwatch);

  puts("===============");
  puts("All tests done.");
  printf("Time taken: %Lg seconds\n", tloStopwatchNumSeconds(&stopwatch));
  puts("===============");

  tloTestPrintReport();
  tloTestExit();
}
