#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlo/darray.h>
#include <tlo/hash.h>
#include <tlo/statistics.h>
#include "hash_benchmark_utils.h"

enum { BUFFER_SIZE = 128 };

static void checkCollisions(TloHashFunction hashFunction, size_t numBuckets,
                            TloList *lines, const char *description) {
  CollisionsData data;

  if (collisionsDataConstruct(&data, description, numBuckets) == TLO_ERROR) {
    return;
  }

  for (size_t i = 0; i < tlovListSize(lines); ++i) {
    const TloCString *cstring = tlovListElement(lines, i);
    size_t hash = hashFunction(*cstring, strlen(*cstring));
    collisionsDataAddHash(&data, hash);
  }

  collisionsDataComputeFinalStats(&data);
  collisionsDataPrintReport(&data);
  collisionsDataDestruct(&data);
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

    size_t size = strlen(line);

    for (size_t i = size - 1; i < size; --i) {
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
  CHECK_COLLISIONS(tloFNV1Hash, numBuckets, lines);
  CHECK_COLLISIONS(tloFNV1aHash, numBuckets, lines);
  CHECK_COLLISIONS(tloOAATHash, numBuckets, lines);
  CHECK_COLLISIONS(tloELFHash, numBuckets, lines);
  CHECK_COLLISIONS(tloPJWHash, numBuckets, lines);

  tloListDelete(lines);
}
