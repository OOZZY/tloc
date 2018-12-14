#include "tloc_test.h"
#include <stdio.h>
#include <tlo/test.h>

int main(void) {
  testList();
  testDArray();
  testSLList();
  testCDArray();
  puts("===============");
  puts("All tests done.");
  puts("===============");
  tloTestPrintReport();
  tloTestExit();
}
