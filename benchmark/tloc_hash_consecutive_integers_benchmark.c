#include <stdio.h>
#include <stdlib.h>
#include <tlo/hash.h>
#include <tlo/statistics.h>
#include "hash_benchmark_utils.h"

static void checkCollisions(TloHashFunction hashFunction, size_t numBuckets,
                            size_t numElements, const char *description) {
  CollisionsData data;

  if (collisionsDataConstruct(&data, description, numBuckets) != TLO_SUCCESS) {
    return;
  }

  for (size_t i = 0; i < numElements; ++i) {
    size_t hash = hashFunction(&i, sizeof(i));
    collisionsDataAddHash(&data, hash);
  }

  collisionsDataComputeFinalStats(&data);
  collisionsDataPrintReport(&data);
  collisionsDataDestruct(&data);
}

#define CHECK_COLLISIONS(_hashFunction, _numBuckets, _numElements) \
  checkCollisions(_hashFunction, _numBuckets, _numElements, #_hashFunction)

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <num-buckets> <num-elements>\n", argv[0]);
    return 1;
  }

  size_t numBuckets = strtoull(argv[1], NULL, 10);
  if (numBuckets < 1) {
    puts("error: given number of buckets is invalid");
    return 1;
  }

  size_t numElements = strtoull(argv[2], NULL, 10);
  if (numElements < 1) {
    puts("error: given number of elements is invalid");
    return 1;
  }

  CHECK_COLLISIONS(tloRotatingHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloDJBHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloMDJBHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloSAXHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloFNV1Hash, numBuckets, numElements);
  CHECK_COLLISIONS(tloFNV1aHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloOAATHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloELFHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloPJWHash, numBuckets, numElements);
}
