#include "hash_benchmark_utils.h"
#include <stdio.h>
#include <stdlib.h>

TloError collisionsDataConstruct(CollisionsData *data, const char *description,
                                 size_t numBuckets) {
  data->description = description;
  data->numBuckets = numBuckets;
  data->numHashes = 0;

  data->bucketSizes = malloc(numBuckets * sizeof(*data->bucketSizes));
  if (!data->bucketSizes) {
    puts("error: failed to allocate array for bucket sizes");
    return TLO_ERROR;
  }

  for (size_t i = 0; i < numBuckets; ++i) {
    data->bucketSizes[i] = 0;
  }

  return TLO_SUCCESS;
}

void collisionsDataAddHash(CollisionsData *data, size_t hash) {
  size_t index = hash % data->numBuckets;
  data->bucketSizes[index]++;

  data->numHashes++;

  if (data->numHashes == 1) {
    data->smallestHash = hash;
    data->largestHash = hash;
  } else {
    if (data->smallestHash > hash) {
      data->smallestHash = hash;
    }

    if (data->largestHash < hash) {
      data->largestHash = hash;
    }
  }
}

void collisionsDataComputeFinalStats(CollisionsData *data) {
  tloStatAccConstruct(&data->bucketSizeAcc);
  data->numCollisions = 0;

  for (size_t i = 0; i < data->numBuckets; ++i) {
    tloStatAccAdd(&data->bucketSizeAcc, data->bucketSizes[i]);

    if (data->bucketSizes[i] > 1) {
      data->numCollisions += data->bucketSizes[i] - 1;
    }
  }
}

void collisionsDataPrintReport(const CollisionsData *data) {
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
  printf("Smallest hash       : %zu\n", data->smallestHash);
  printf("Largest hash        : %zu\n", data->largestHash);
  puts("====================");
}

void collisionsDataDestruct(CollisionsData *data) { free(data->bucketSizes); }
