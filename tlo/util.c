#include "tlo/util.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static TloError intConstructCopy(void *bytes, const void *data) {
  assert(bytes);
  assert(data);

  memcpy(bytes, data, sizeof(int));

  return 0;
}

static void basicDestruct(void *bytes) { (void)bytes; }

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

const TloType tloInt = {.sizeOf = sizeof(int),
                        .constructCopy = intConstructCopy,
                        .destruct = basicDestruct};

bool tloAllocatorTypeIsValid(const TloAllocatorType *allocatorType) {
  return allocatorType && allocatorType->malloc && allocatorType->free;
}

const TloAllocatorType tloCStdLibAllocator = {.malloc = malloc, .free = free};

const TloAllocatorType tloCountingAllocator = {
    .malloc = countingAllocatorMalloc, .free = countingAllocatorFree};

void tloCountingAllocatorResetCounts(void) {
  countingAllocatorMallocCount = 0;
  countingAllocatorFreeCount = 0;
  countingAllocatorTotalByteCount = 0;
}

unsigned long tloCountingAllocatorGetMallocCount(void) {
  return countingAllocatorMallocCount;
}

unsigned long tloCountingAllocatorGetFreeCount(void) {
  return countingAllocatorFreeCount;
}

unsigned long tloCountingAllocatorGetTotalByteCount(void) {
  return countingAllocatorTotalByteCount;
}
