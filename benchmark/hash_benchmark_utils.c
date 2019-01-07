#include "hash_benchmark_utils.h"
#include <stdio.h>

void printCollisionsReport(const CollisionsData *data) {
  puts("====================");
  puts(data->description);
  printf("Number of buckets   : %zu\n", tloStatAccSize(&data->bucketSizeAcc));
  printf("Number of elements  : %Lg\n", tloStatAccSum(&data->bucketSizeAcc));
  printf("Average bucket size : %Lg\n", tloStatAccMean(&data->bucketSizeAcc));
  printf("Smallest bucket size: %Lg\n",
         tloStatAccMinimum(&data->bucketSizeAcc));
  printf("Largest bucket size : %Lg\n",
         tloStatAccMaximum(&data->bucketSizeAcc));
  printf("Range               : %Lg\n", tloStatAccRange(&data->bucketSizeAcc));
  printf("Variance            : %Lg\n",
         tloStatAccVariance(&data->bucketSizeAcc));
  printf("Standard deviation  : %Lg\n",
         tloStatAccStandardDeviation(&data->bucketSizeAcc));
  printf("Number of collisions: %zu\n", data->numCollisions);
  puts("====================");
}
