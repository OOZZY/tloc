#include "util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long mallocCount = 0;
static unsigned long freeCount = 0;
static unsigned long totalByteCount = 0;

static void *countingAllocatorMalloc(size_t size) {
  ++mallocCount;
  void *memory = malloc(size);
  if (memory) {
    totalByteCount += size;
  }
  return memory;
}

static void countingAllocatorFree(void *memory) {
  ++freeCount;
  free(memory);
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

int *makeInt(int value) {
  int *i = countingAllocatorMalloc(sizeof(*i));
  if (!i) {
    return NULL;
  }

  *i = value;
  return i;
}

TloError intPtrConstruct(IntPtr *ptr, int value) {
  assert(ptr);

  int *newPtr = malloc(sizeof(int));
  if (!newPtr) {
    return TLO_ERROR;
  }

  *newPtr = value;
  ptr->ptr = newPtr;

  return TLO_SUCCESS;
}

TloError intPtrConstructCopy(IntPtr *ptr, const IntPtr *other) {
  assert(ptr);
  assert(other && other->ptr);

  if (intPtrConstruct(ptr, 0) == TLO_ERROR) {
    return TLO_ERROR;
  }

  *ptr->ptr = *other->ptr;

  return TLO_SUCCESS;
}

IntPtr *intPtrMake(int value) {
  IntPtr *ptr = countingAllocatorMalloc(sizeof(*ptr));
  if (!ptr) {
    return NULL;
  }

  if (intPtrConstruct(ptr, value) == TLO_ERROR) {
    countingAllocatorFree(ptr);
    return NULL;
  }

  return ptr;
}

static TloError intPtrTypeConstructCopy(void *destination, const void *source) {
  return intPtrConstructCopy(destination, source);
}

const TloType intPtrType = {.size = sizeof(IntPtr),
                            .constructCopy = intPtrTypeConstructCopy,
                            .destruct = tloPtrDestruct};
