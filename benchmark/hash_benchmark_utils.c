#include "hash_benchmark_utils.h"
#include <stdio.h>

void collisionsDataConstruct(CollisionsData *data, const char *description,
                             size_t *bucketSizes, size_t numBuckets) {
  data->description = description;
  tloStatAccConstruct(&data->bucketSizeAcc);
  data->numCollisions = 0;

  for (size_t i = 0; i < numBuckets; ++i) {
    tloStatAccAdd(&data->bucketSizeAcc, bucketSizes[i]);

    if (bucketSizes[i] > 1) {
      data->numCollisions += bucketSizes[i] - 1;
    }
  }
}

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
