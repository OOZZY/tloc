#include "statistics_test.h"
#include <stdio.h>
#include <tlo/statistics.h>
#include <tlo/test.h>

static void constructorShouldInitializeSize() {
  TloStatAccumulator accumulator;
  tloStatAccConstruct(&accumulator);
  TLO_EXPECT(tloStatAccSize(&accumulator) == 0);
}

static void addShouldUpdateStatistics() {
  TloStatAccumulator accumulator;
  tloStatAccConstruct(&accumulator);

  tloStatAccAdd(&accumulator, 50);
  TLO_EXPECT(tloStatAccSize(&accumulator) == 1);
  TLO_EXPECT((int)(tloStatAccSum(&accumulator)) == 50);
  TLO_EXPECT((int)(tloStatAccMean(&accumulator)) == 50);
  TLO_EXPECT((int)(tloStatAccVariance(&accumulator)) == 0);
  TLO_EXPECT((int)(tloStatAccStandardDeviation(&accumulator)) == 0);
  TLO_EXPECT((int)(tloStatAccMinimum(&accumulator)) == 50);
  TLO_EXPECT((int)(tloStatAccMaximum(&accumulator)) == 50);
  TLO_EXPECT((int)(tloStatAccRange(&accumulator)) == 0);

  tloStatAccAdd(&accumulator, 100);
  TLO_EXPECT(tloStatAccSize(&accumulator) == 2);
  TLO_EXPECT((int)(tloStatAccSum(&accumulator)) == 150);
  TLO_EXPECT((int)(tloStatAccMean(&accumulator)) == 75);
  TLO_EXPECT((int)(tloStatAccVariance(&accumulator)) == 625);
  TLO_EXPECT((int)(tloStatAccStandardDeviation(&accumulator)) == 25);
  TLO_EXPECT((int)(tloStatAccMinimum(&accumulator)) == 50);
  TLO_EXPECT((int)(tloStatAccMaximum(&accumulator)) == 100);
  TLO_EXPECT((int)(tloStatAccRange(&accumulator)) == 50);

  tloStatAccAdd(&accumulator, 25);
  TLO_EXPECT(tloStatAccSize(&accumulator) == 3);
  TLO_EXPECT((int)(tloStatAccSum(&accumulator)) == 175);
  TLO_EXPECT((int)(tloStatAccMean(&accumulator)) == 58);
  TLO_EXPECT((int)(tloStatAccVariance(&accumulator)) == 972);
  TLO_EXPECT((int)(tloStatAccStandardDeviation(&accumulator)) == 31);
  TLO_EXPECT((int)(tloStatAccMinimum(&accumulator)) == 25);
  TLO_EXPECT((int)(tloStatAccMaximum(&accumulator)) == 100);
  TLO_EXPECT((int)(tloStatAccRange(&accumulator)) == 75);
}

void testStatistics(void) {
  constructorShouldInitializeSize();
  addShouldUpdateStatistics();
  puts("======================");
  puts("Statistics tests done.");
  puts("======================");
}
