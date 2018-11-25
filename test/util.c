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

const TloAllocatorType countingAllocator = {.malloc = countingAllocatorMalloc,
                                            .free = countingAllocatorFree};

void countingAllocatorResetCounts(void) {
  mallocCount = 0;
  freeCount = 0;
  totalByteCount = 0;
}

unsigned long countingAllocatorMallocCount(void) { return mallocCount; }

unsigned long countingAllocatorFreeCount(void) { return freeCount; }

unsigned long countingAllocatorTotalByteCount(void) { return totalByteCount; }

void countingAllocatorPrintCounts(void) {
  printf("malloc count: %lu\n", mallocCount);
  printf("free count: %lu\n", freeCount);
  printf("Total bytes allocated: %lu\n", totalByteCount);
}

TloError intPtrConstruct(IntPtr *ptr) {
  assert(ptr);

  int *newPtr = malloc(sizeof(int));
  if (!newPtr) {
    return TLO_ERROR;
  }

  ptr->ptr = newPtr;

  return TLO_SUCCESS;
}

TloError intPtrConstructCopy(IntPtr *ptr, const IntPtr *other) {
  assert(ptr);
  assert(other && other->ptr);

  if (intPtrConstruct(ptr) == TLO_ERROR) {
    return TLO_ERROR;
  }

  *ptr->ptr = *other->ptr;

  return TLO_SUCCESS;
}

static TloError intPtrTypeConstructCopy(void *bytes, const void *data) {
  return intPtrConstructCopy(bytes, data);
}

const TloType intPtrType = {.sizeOf = sizeof(IntPtr),
                            .constructCopy = intPtrTypeConstructCopy,
                            .destruct = tloPtrDestruct};
