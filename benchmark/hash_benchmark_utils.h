#ifndef BENCHMARK_HASH_BENCHMARK_UTILS_H
#define BENCHMARK_HASH_BENCHMARK_UTILS_H

#include <tlo/statistics.h>
#include <tlo/util.h>

typedef struct CollisionsData {
  const char *description;
  size_t *bucketSizes;
  size_t numBuckets;
  TloStatAccumulator bucketSizeAcc;
  size_t numCollisions;
  size_t numHashes;
  size_t smallestHash;
  size_t largestHash;
} CollisionsData;

TloError collisionsDataConstruct(CollisionsData *data, const char *description,
                                 size_t numBuckets);
void collisionsDataAddHash(CollisionsData *data, size_t hash);
void collisionsDataComputeFinalStats(CollisionsData *data);
void collisionsDataPrintReport(const CollisionsData *data);
void collisionsDataDestruct(CollisionsData *data);

#endif  // BENCHMARK_HASH_BENCHMARK_UTILS_H
