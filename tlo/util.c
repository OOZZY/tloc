#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static tloError intTypeConstructCopy(void *bytes, const void *data) {
  assert(bytes);
  assert(data);

  memcpy(bytes, data, sizeof(int));

  return 0;
}

static void basicTypeDestruct(void *bytes) { (void)bytes; }

static unsigned long countingAllocatorMallocCount = 0;
static unsigned long countingAllocatorFreeCount = 0;
static unsigned long countingAllocatorTotalByteCount = 0;

static void *countingAllocatorMalloc(size_t byteCount) {
  ++countingAllocatorMallocCount;
  void *bytes = malloc(byteCount);
  if (bytes) {
    countingAllocatorTotalByteCount += byteCount;
  }
  return bytes;
}

static void countingAllocatorFree(void *bytes) {
  ++countingAllocatorFreeCount;
  free(bytes);
}

bool tloTypeIsValid(const TloType *type) {
  return type && type->sizeOf && type->constructCopy && type->destruct;
}

const TloType tloIntType = {.sizeOf = sizeof(int),
                            .constructCopy = intTypeConstructCopy,
                            .destruct = basicTypeDestruct};

bool tloAllocatorIsValid(const TloAllocator *allocator) {
  return allocator && allocator->malloc && allocator->free;
}

const TloAllocator tloCStdLibAllocator = {.malloc = malloc, .free = free};

const TloAllocator tloCountingAllocator = {.malloc = countingAllocatorMalloc,
                                           .free = countingAllocatorFree};

unsigned long tloCountingAllocatorGetMallocCount(void) {
  return countingAllocatorMallocCount;
}

unsigned long tloCountingAllocatorGetFreeCount(void) {
  return countingAllocatorFreeCount;
}

unsigned long tloCountingAllocatorGetTotalByteCount(void) {
  return countingAllocatorTotalByteCount;
}
