#include <stdio.h>
#include <stdlib.h>
#include <tlo/hash.h>
#include <tlo/statistics.h>

static void printReport(const char *description,
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

static void checkCollisions(TloHashFunction hashFunction, size_t numBuckets,
                            size_t numElements, const char *description) {
  size_t *bucketSizes = malloc(numBuckets * sizeof(*bucketSizes));
  if (!bucketSizes) {
    puts("error: failed to allocate array for bucket sizes");
    return;
  }

  for (size_t i = 0; i < numBuckets; ++i) {
    bucketSizes[i] = 0;
  }

  for (size_t i = 0; i < numElements; ++i) {
    size_t hash = hashFunction(&i, sizeof(i));
    size_t index = hash % numBuckets;
    bucketSizes[index]++;
  }

  TloStatAccumulator accumulator;
  tloStatAccConstruct(&accumulator);

  size_t numCollisions = 0;

  for (size_t i = 0; i < numBuckets; ++i) {
    tloStatAccAdd(&accumulator, bucketSizes[i]);

    if (bucketSizes[i] > 1) {
      numCollisions += bucketSizes[i] - 1;
    }
  }

  printReport(description, &accumulator, numCollisions);
  free(bucketSizes);
}

#define CHECK_COLLISIONS(_hashFunction, _numBuckets, _numElements) \
  checkCollisions(_hashFunction, _numBuckets, _numElements, #_hashFunction);

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
  CHECK_COLLISIONS(tloFNVHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloOAATHash, numBuckets, numElements);
  CHECK_COLLISIONS(tloELFHash, numBuckets, numElements);
}
