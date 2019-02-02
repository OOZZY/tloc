#include "util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlo/test.h>

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

const TloAllocator countingAllocator = {.malloc = countingAllocatorMalloc,
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

void testInitialCounts(void) {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

void testFinalCounts(void) {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);
  countingAllocatorPrintCounts();
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

  if (intPtrConstruct(ptr, 0) != TLO_SUCCESS) {
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

  if (intPtrConstruct(ptr, value) != TLO_SUCCESS) {
    countingAllocatorFree(ptr);
    return NULL;
  }

  return ptr;
}

bool intPtrEquals(const IntPtr *ptr1, const IntPtr *ptr2) {
  assert(ptr1 && ptr1->ptr);
  assert(ptr2 && ptr2->ptr);

  return *ptr1->ptr == *ptr2->ptr;
}

size_t intPtrHash(const IntPtr *ptr) {
  assert(ptr && ptr->ptr);

  return tloFNV1aHash(ptr->ptr, sizeof(int));
}

int intPtrCompare(const IntPtr *ptr1, const IntPtr *ptr2) {
  assert(ptr1 && ptr1->ptr);
  assert(ptr2 && ptr2->ptr);

  if (*ptr1->ptr < *ptr2->ptr) {
    return -1;
  } else if (*ptr1->ptr > *ptr2->ptr) {
    return 1;
  } else {
    return 0;
  }
}

static TloError intPtrTypeConstructCopy(void *destination, const void *source) {
  return intPtrConstructCopy(destination, source);
}

static bool intPtrTypeEquals(const void *object1, const void *object2) {
  return intPtrEquals(object1, object2);
}

static size_t intPtrTypeHash(const void *object, size_t size) {
  (void)size;
  return intPtrHash(object);
}

static int intPtrTypeCompare(const void *object1, const void *object2) {
  return intPtrCompare(object1, object2);
}

const TloType intPtrType = {.size = sizeof(IntPtr),
                            .constructCopy = intPtrTypeConstructCopy,
                            .destruct = tloPtrDestruct,
                            .equals = intPtrTypeEquals,
                            .hash = intPtrTypeHash,
                            .compare = intPtrTypeCompare};
