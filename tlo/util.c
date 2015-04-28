#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static int intTypeConstructCopy(void *bytes, const void *data) {
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

bool tloTypeIsValid(const tloType *type) {
  return type && type->sizeOf && type->constructCopy && type->destruct;
}

const tloType tloIntType = {.sizeOf = sizeof(int),
                            .constructCopy = intTypeConstructCopy,
                            .destruct = basicTypeDestruct};

bool tloAllocatorIsValid(const tloAllocator *allocator) {
  return allocator && allocator->malloc && allocator->free;
}

const tloAllocator tloCStdLibAllocator = {.malloc = malloc, .free = free};

const tloAllocator tloCountingAllocator = {.malloc = countingAllocatorMalloc,
                                           .free = countingAllocatorFree};

unsigned long tloCountingAllocatorGetMallocCount() {
  return countingAllocatorMallocCount;
}

unsigned long tloCountingAllocatorGetFreeCount() {
  return countingAllocatorFreeCount;
}

unsigned long tloCountingAllocatorGetTotalByteCount() {
  return countingAllocatorTotalByteCount;
}
