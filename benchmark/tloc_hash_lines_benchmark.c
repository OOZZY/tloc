#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlo/darray.h>
#include <tlo/hash.h>
#include <tlo/statistics.h>

enum { BUFFER_SIZE = 128 };

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
                            TloList *lines, const char *description) {
  size_t *bucketSizes = malloc(numBuckets * sizeof(*bucketSizes));
  if (!bucketSizes) {
    puts("error: failed to allocate array for bucket sizes");
    return;
  }

  for (size_t i = 0; i < numBuckets; ++i) {
    bucketSizes[i] = 0;
  }

  for (size_t i = 0; i < tlovListSize(lines); ++i) {
    const TloCString *cstring = tlovListElement(lines, i);
    size_t hash = hashFunction(*cstring, strlen(*cstring));
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

#define CHECK_COLLISIONS(_hashFunction, _numBuckets, _lines) \
  checkCollisions(_hashFunction, _numBuckets, _lines, #_hashFunction);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: %s <num-buckets> < <text-file>\n", argv[0]);
    puts("- assumes each line of the text file is a unique string");
    return 1;
  }

  size_t numBuckets = strtoull(argv[1], NULL, 10);
  if (numBuckets < 1) {
    puts("error: given number of buckets is invalid");
    return 1;
  }

  TloList *lines;
  char buffer[BUFFER_SIZE];

  lines = (TloList *)tloDArrayMake(&tloCString, NULL, 0);
  if (!lines) {
    puts("error: failed to allocate/construct array for lines");
    return 1;
  }

  while (1) {
    TloCString line = fgets(buffer, BUFFER_SIZE, stdin);
    if (!line) {
      break;
    }

    size_t length = strlen(line);

    for (size_t i = length - 1; i < length; --i) {
      if (line[i] != '\n' && line[i] != '\r') {
        break;
      }

      line[i] = '\0';
    }

    tlovListPushBack(lines, &line);
  }

  CHECK_COLLISIONS(tloRotatingHash, numBuckets, lines);
  CHECK_COLLISIONS(tloDJBHash, numBuckets, lines);
  CHECK_COLLISIONS(tloMDJBHash, numBuckets, lines);
  CHECK_COLLISIONS(tloSAXHash, numBuckets, lines);
  CHECK_COLLISIONS(tloFNVHash, numBuckets, lines);
  CHECK_COLLISIONS(tloOAATHash, numBuckets, lines);
  CHECK_COLLISIONS(tloELFHash, numBuckets, lines);

  tloListDelete(lines);
}
