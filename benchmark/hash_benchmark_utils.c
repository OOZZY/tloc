#include "hash_benchmark_utils.h"
#include <stdio.h>

void printCollisionsReport(const char *description,
                           const TloStatAccumulator *accumulator,
                           size_t numCollisions) {
  puts("====================");
  puts(description);
  printf("Number of buckets   : %zu\n", tloStatAccSize(accumulator));
  printf("Number of elements  : %Lg\n", tloStatAccSum(accumulator));
  printf("Average bucket size : %Lg\n", tloStatAccMean(accumulator));
  printf("Smallest bucket size: %Lg\n", tloStatAccMinimum(accumulator));
  printf("Largest bucket size : %Lg\n", tloStatAccMaximum(accumulator));
  printf("Range               : %Lg\n", tloStatAccRange(accumulator));
  printf("Variance            : %Lg\n", tloStatAccVariance(accumulator));
  printf("Standard deviation  : %Lg\n",
         tloStatAccStandardDeviation(accumulator));
  printf("Number of collisions: %zu\n", numCollisions);
  puts("====================");
}
