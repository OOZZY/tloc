#include "tloc_test.h"
#include <stdio.h>
#include <tlo/test.h>

int main(void) {
  testDArray();
  testSLList();
  puts("===============");
  puts("All tests done.");
  puts("===============");
  tloTestPrintReport();
  tloTestExit();
}
