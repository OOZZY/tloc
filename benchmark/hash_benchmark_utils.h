#ifndef BENCHMARK_HASH_BENCHMARK_UTILS_H
#define BENCHMARK_HASH_BENCHMARK_UTILS_H

#include <tlo/statistics.h>

typedef struct CollisionsData {
  const char *description;
  TloStatAccumulator bucketSizeAcc;
  size_t numCollisions;
} CollisionsData;

void printCollisionsReport(const CollisionsData *data);

#endif  // BENCHMARK_HASH_BENCHMARK_UTILS_H
