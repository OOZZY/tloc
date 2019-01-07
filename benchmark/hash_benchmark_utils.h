#ifndef BENCHMARK_HASH_BENCHMARK_UTILS_H
#define BENCHMARK_HASH_BENCHMARK_UTILS_H

#include <tlo/statistics.h>

void printCollisionsReport(const char *description,
                           const TloStatAccumulator *accumulator,
                           size_t numCollisions);

#endif  // BENCHMARK_HASH_BENCHMARK_UTILS_H
