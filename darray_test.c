#include "libtloc_test.h"
#include <tlo/darray.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define SOME_NUMBER 42

static void testDArrayIntConstructDestructStackSpace(void) {
  TloDArray ints;

  tloError error =
      tloDArrayConstruct(&ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) >= tloDArrayGetSize(&ints));
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetType(&ints) == &tloIntType);
  assert(tloDArrayGetAllocator(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  tloError error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntConstructWithCapacityDestructStackSpace(void) {
  TloDArray ints;

  tloError error = tloDArrayConstructWithCapacity(
      &ints, &tloIntType, &tloCountingAllocator, SOME_NUMBER);
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetType(&ints) == &tloIntType);
  assert(tloDArrayGetAllocator(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructWithCapacityDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  tloError error = tloDArrayConstructWithCapacity(
      ints, &tloIntType, &tloCountingAllocator, SOME_NUMBER);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints = tloDArrayMakeWithCapacity(
      &tloIntType, &tloCountingAllocator, SOME_NUMBER);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayDestructWithNull(void) { tloDArrayDestruct(NULL); }

static void testDArrayDeleteWithNull(void) { tloDArrayDelete(NULL); }

static void testDArrayIntPushBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  tloError error = tloDArrayPushBack(ints, &(int) {SOME_NUMBER});
  assert(!error);

  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMoveBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  tloError error = tloDArrayMoveBack(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResize(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloDArrayPushBack(ints, &(int) {(int)i});
    assert(!error);

    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int)i);
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int)i);
  }

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMoveBackUntilResize(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    tloError error = tloDArrayMoveBack(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int)i);
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int)i);
  }

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackOncePopBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  tloError error = tloDArrayPushBack(ints, &(int) {SOME_NUMBER});
  assert(!error);

  tloDArrayPopBack(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloDArrayPushBack(ints, &(int) {(int)i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int)i);
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int)i);

    tloDArrayPopBack(ints);
  }

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntConstructCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = malloc(sizeof(*copy));
  assert(copy);

  tloError error = tloDArrayConstructCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDestruct(copy);
  free(copy);
  copy = NULL;
}

static void testDArrayIntMakeCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = tloDArrayMakeCopy(ints);
  assert(copy);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
}

static void testDArrayIntCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(copy);

  tloError error = tloDArrayCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
}

void testDArray(void) {
  tloCountingAllocatorResetCounts();
  assert(tloCountingAllocatorGetMallocCount() == 0);
  assert(tloCountingAllocatorGetMallocCount() ==
         tloCountingAllocatorGetFreeCount());
  assert(tloCountingAllocatorGetTotalByteCount() == 0);

  testDArrayIntConstructDestructStackSpace();
  testDArrayIntConstructDestructHeapSpace();
  testDArrayIntConstructWithCapacityDestructStackSpace();
  testDArrayIntConstructWithCapacityDestructHeapSpace();
  testDArrayIntMakeDelete();
  testDArrayIntMakeWithCapacityDelete();
  testDArrayDestructWithNull();
  testDArrayDeleteWithNull();
  testDArrayIntPushBackOnce();
  testDArrayIntMoveBackOnce();
  testDArrayIntPushBackUntilResize();
  testDArrayIntMoveBackUntilResize();
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntConstructCopy();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();

  assert(tloCountingAllocatorGetMallocCount() > 0);
  assert(tloCountingAllocatorGetMallocCount() ==
         tloCountingAllocatorGetFreeCount());
  assert(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloDArray): %zu\n", sizeof(TloDArray));
  printf("malloc count: %lu\nfree count: %lu\ntotal bytes allocated: %lu\n",
         tloCountingAllocatorGetMallocCount(),
         tloCountingAllocatorGetFreeCount(),
         tloCountingAllocatorGetTotalByteCount());
  puts("DArray tests passed.");
}
