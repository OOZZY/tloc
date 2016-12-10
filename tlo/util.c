#include "tlo/util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool tloTypeIsValid(const TloType *type) { return type && type->sizeOf; }

const TloType tloInt = {
    .sizeOf = sizeof(int), .constructCopy = NULL, .destruct = NULL};

bool tloAllocatorTypeIsValid(const TloAllocatorType *allocatorType) {
  return allocatorType && allocatorType->malloc && allocatorType->free;
}

const TloAllocatorType tloCStdLibAllocator = {.malloc = malloc, .free = free};

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

void tloCountingAllocatorPrintCounts(void) {
  printf("malloc count: %lu\nfree count: %lu\nTotal bytes allocated: %lu\n",
         countingAllocatorMallocCount, countingAllocatorFreeCount,
         countingAllocatorTotalByteCount);
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
