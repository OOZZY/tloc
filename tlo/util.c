#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool tloTypeIsValid(const tloType *type) {
  return (
    (type) &&
    (type->sizeOf) &&
    (type->constructCopy) &&
    (type->destruct)
  );
}

int tloIntTypeConstructCopy(void *bytes, const void *data) {
  assert(bytes);
  assert(data);

  memcpy(bytes, data, sizeof(int));

  return 0;
}

void tloIntTypeDestruct(void *bytes) {
  (void)bytes;
}

const tloType tloIntType = {
  .sizeOf = sizeof(int),
  .constructCopy = tloIntTypeConstructCopy,
  .destruct = tloIntTypeDestruct
};

bool tloAllocatorIsValid(const tloAllocator *allocator) {
  return (
    (allocator) &&
    (allocator->malloc) &&
    (allocator->free)
  );
}

const tloAllocator tloCStdLibAllocator = {
  .malloc = malloc,
  .free = free
};

unsigned long tloCountingAllocatorMallocCount = 0;
unsigned long tloCountingAllocatorFreeCount = 0;
unsigned long tloCountingAllocatorTotalByteCount = 0;

void *tloCountingAllocatorMalloc(size_t byteCount) {
  ++tloCountingAllocatorMallocCount;
  void *bytes = malloc(byteCount);
  if (bytes) {
    tloCountingAllocatorTotalByteCount += byteCount;
  }
  return bytes;
}

void tloCountingAllocatorFree(void *bytes) {
  ++tloCountingAllocatorFreeCount;
  free(bytes);
}

const tloAllocator tloCountingAllocator = {
  .malloc = tloCountingAllocatorMalloc,
  .free = tloCountingAllocatorFree
};
