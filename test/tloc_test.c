#include <stdio.h>
#include <tlo/test.h>
#include "cdarray_test.h"
#include "darray_test.h"
#include "dllist_test.h"
#include "list_test.h"
#include "sllist_test.h"

int main(void) {
  testList();
  testDArray();
  testSLList();
  testCDArray();
  testDLList();
  puts("===============");
  puts("All tests done.");
  puts("===============");
  tloTestPrintReport();
  tloTestExit();
}
