#include "util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long mallocCount = 0;
static unsigned long freeCount = 0;
static unsigned long totalByteCount = 0;

static void *countingAllocatorMalloc(size_t byteCount) {
  ++mallocCount;
  void *bytes = malloc(byteCount);
  if (bytes) {
    totalByteCount += byteCount;
  }
  return bytes;
}

static void countingAllocatorFree(void *bytes) {
  ++freeCount;
  free(bytes);
}

const TloAllocatorType tloCountingAllocator = {
    .malloc = countingAllocatorMalloc, .free = countingAllocatorFree};

void tloCountingAllocatorResetCounts(void) {
  mallocCount = 0;
  freeCount = 0;
  totalByteCount = 0;
}

unsigned long tloCountingAllocatorMallocCount(void) { return mallocCount; }

unsigned long tloCountingAllocatorFreeCount(void) { return freeCount; }

unsigned long tloCountingAllocatorTotalByteCount(void) {
  return totalByteCount;
}

void tloCountingAllocatorPrintCounts(void) {
  printf("malloc count: %lu\n", mallocCount);
  printf("free count: %lu\n", freeCount);
  printf("Total bytes allocated: %lu\n", totalByteCount);
}

TloError tloIntPtrConstruct(TloIntPtr *ptr) {
  assert(ptr);

  int *newPtr = malloc(sizeof(int));
  if (!newPtr) {
    return TLO_ERROR;
  }

  ptr->ptr = newPtr;

  return TLO_SUCCESS;
}

TloError tloIntPtrConstructCopy(TloIntPtr *ptr, const TloIntPtr *other) {
  assert(ptr);
  assert(other && other->ptr);

  if (tloIntPtrConstruct(ptr) == TLO_ERROR) {
    return TLO_ERROR;
  }

  *ptr->ptr = *other->ptr;

  return TLO_SUCCESS;
}

static TloError intPtrConstructCopy(void *bytes, const void *data) {
  return tloIntPtrConstructCopy(bytes, data);
}

void tloPtrDestruct(void *ptr) {
  int **ptrptr = ptr;

  if (!ptrptr) {
    return;
  }

  if (!*ptrptr) {
    return;
  }

  free(*ptrptr);
  *ptrptr = NULL;
}

const TloType tloIntPtr = {.sizeOf = sizeof(TloIntPtr),
                           .constructCopy = intPtrConstructCopy,
                           .destruct = tloPtrDestruct};

const TloType tloPtr = {
    .sizeOf = sizeof(int *), .constructCopy = NULL, .destruct = tloPtrDestruct};
