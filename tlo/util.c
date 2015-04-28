#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool tloTypeIsValid(const tloType *type) {
  return ((type) && (type->sizeOf) && (type->constructCopy) &&
          (type->destruct));
}

int tloIntTypeConstructCopy(void *bytes, const void *data) {
  assert(bytes);
  assert(data);

  memcpy(bytes, data, sizeof(int));

  return 0;
}

void tloBasicTypeDestruct(void *bytes) { (void)bytes; }

const tloType tloIntType = {.sizeOf = sizeof(int),
                            .constructCopy = tloIntTypeConstructCopy,
                            .destruct = tloBasicTypeDestruct};

bool tloAllocatorIsValid(const tloAllocator *allocator) {
  return ((allocator) && (allocator->malloc) && (allocator->free));
}

const tloAllocator tloCStdLibAllocator = {.malloc = malloc, .free = free};

static unsigned long countingAllocatorMallocCount = 0;
static unsigned long countingAllocatorFreeCount = 0;
static unsigned long countingAllocatorTotalByteCount = 0;

void *tloCountingAllocatorMalloc(size_t byteCount) {
  ++countingAllocatorMallocCount;
  void *bytes = malloc(byteCount);
  if (bytes) {
    countingAllocatorTotalByteCount += byteCount;
  }
  return bytes;
}

void tloCountingAllocatorFree(void *bytes) {
  ++countingAllocatorFreeCount;
  free(bytes);
}

const tloAllocator tloCountingAllocator = {.malloc = tloCountingAllocatorMalloc,
                                           .free = tloCountingAllocatorFree};

unsigned long tloCountingAllocatorGetMallocCount() {
  return countingAllocatorMallocCount;
}

unsigned long tloCountingAllocatorGetFreeCount() {
  return countingAllocatorFreeCount;
}

unsigned long tloCountingAllocatorGetTotalByteCount() {
  return countingAllocatorTotalByteCount;
}
