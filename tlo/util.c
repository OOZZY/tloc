#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool tloTypeIsValid(const tloType *type) {
  return (
    (type) &&
    (type->sizeOf) &&
    (type->copyConstruct) &&
    (type->destruct)
  );
}

int tloIntTypeCopyConstruct(void *obj, const void *data) {
  assert(obj);
  assert(data);

  memcpy(obj, data, sizeof(int));

  return 0;
}

void tloIntTypeDestruct(void *obj) {
  (void)obj;
}

const tloType tloIntType = {
  .sizeOf = sizeof(int),
  .copyConstruct = tloIntTypeCopyConstruct,
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

void *tloCountingAllocatorMalloc(size_t byteCount) {
  ++tloCountingAllocatorMallocCount;
  return malloc(byteCount);
}

void tloCountingAllocatorFree(void *bytes) {
  ++tloCountingAllocatorFreeCount;
  free(bytes);
}

const tloAllocator tloCountingAllocator = {
  .malloc = tloCountingAllocatorMalloc,
  .free = tloCountingAllocatorFree
};
