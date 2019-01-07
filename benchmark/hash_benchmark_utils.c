#include "hash_benchmark_utils.h"
#include <stdio.h>

void printCollisionsReport(const CollisionsData *data) {
  puts("====================");
  puts(data->description);
  printf("Number of buckets   : %zu\n", tloStatAccSize(&data->accumulator));
  printf("Number of elements  : %Lg\n", tloStatAccSum(&data->accumulator));
  printf("Average bucket size : %Lg\n", tloStatAccMean(&data->accumulator));
  printf("Smallest bucket size: %Lg\n", tloStatAccMinimum(&data->accumulator));
  printf("Largest bucket size : %Lg\n", tloStatAccMaximum(&data->accumulator));
  printf("Range               : %Lg\n", tloStatAccRange(&data->accumulator));
  printf("Variance            : %Lg\n", tloStatAccVariance(&data->accumulator));
  printf("Standard deviation  : %Lg\n",
         tloStatAccStandardDeviation(&data->accumulator));
  printf("Number of collisions: %zu\n", data->numCollisions);
  puts("====================");
}
