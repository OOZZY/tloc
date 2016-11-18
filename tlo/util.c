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

static TloError intPtrConstructCopy(void *bytes, const void *data) {
  return tloIntPtrConstructCopy(bytes, data);
}

static void intPtrDestruct(void *bytes) { tloIntPtrDestruct(bytes); }

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

TloError tloIntPtrConstruct(TloIntPtr *ptr) {
  assert(ptr);

  int *newPtr = malloc(sizeof(int));
  if (!newPtr) {
    return -1;
  }

  ptr->ptr = newPtr;

  return 0;
}

TloError tloIntPtrConstructCopy(TloIntPtr *ptr, const TloIntPtr *other) {
  assert(ptr);
  assert(other && other->ptr);

  if (tloIntPtrConstruct(ptr) == -1) {
    return -1;
  }

  *ptr->ptr = *other->ptr;

  return 0;
}

void tloIntPtrDestruct(TloIntPtr *ptr) {
  if (!ptr) {
    return;
  }

  if (!ptr->ptr) {
    return;
  }

  free(ptr->ptr);
  ptr->ptr = NULL;
}

const TloType tloIntPtr = {.sizeOf = sizeof(TloIntPtr),
                           .constructCopy = intPtrConstructCopy,
                           .destruct = intPtrDestruct};
