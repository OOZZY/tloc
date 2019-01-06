#include "tlo/benchmark.h"
#include <assert.h>
#include <stdio.h>
#include "tlo/statistics.h"
#include "tlo/stopwatch.h"

static void printReport(const char *description,
                        const TloStatAccumulator *accumulator) {
  puts("====================");
  puts(description);
  printf("Number of iterations: %zu\n", tloStatAccSize(accumulator));
  printf("Total time          : %Lg seconds\n", tloStatAccSum(accumulator));
  printf("Average time        : %Lg seconds\n", tloStatAccMean(accumulator));
  printf("Fastest time        : %Lg seconds\n", tloStatAccMinimum(accumulator));
  printf("Slowest time        : %Lg seconds\n", tloStatAccMaximum(accumulator));
  printf("Range               : %Lg seconds\n", tloStatAccRange(accumulator));
  printf("Variance            : %Lg\n", tloStatAccVariance(accumulator));
  printf("Standard deviation  : %Lg seconds\n",
         tloStatAccStandardDeviation(accumulator));
  puts("====================");
}

void tloTimeTask(TloBenchmarkTask task, const void *parameters,
                 int numIterations, const char *description) {
  assert(task);
  assert(numIterations > 0);
  assert(description);

  TloStopwatch stopwatch;
  TloStatAccumulator accumulator;
  tloStatAccConstruct(&accumulator);

  for (int i = 0; i < numIterations; ++i) {
    tloStopwatchStart(&stopwatch);
    task(parameters);
    tloStopwatchStop(&stopwatch);
    tloStatAccAdd(&accumulator, tloStopwatchNumSeconds(&stopwatch));
  }

  printReport(description, &accumulator);
}
