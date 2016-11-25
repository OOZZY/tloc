#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include "libtloc_test.h"

#define SOME_NUMBER 42

static void testDArrayIntConstructDestructStackSpace(void) {
  TloDArray ints;

  TloError error = tloDArrayConstruct(&ints, &tloInt, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) >= tloDArrayGetSize(&ints));
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetValueType(&ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  TloError error = tloDArrayConstruct(ints, &tloInt, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntConstructWithCapacityDestructStackSpace(void) {
  TloDArray ints;

  TloError error = tloDArrayConstructWithCapacity(
      &ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetValueType(&ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructWithCapacityDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  TloError error = tloDArrayConstructWithCapacity(
      ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints =
      tloDArrayMakeWithCapacity(&tloInt, &tloCountingAllocator, SOME_NUMBER);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayDestructWithNull(void) { tloDArrayDestruct(NULL); }

static void testDArrayDeleteWithNull(void) { tloDArrayDelete(NULL); }

static void testDArrayIntPushBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  TloError error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  TloError error = tloDArrayMoveBack(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &(int){(int)i});
    assert(!error);

    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetValueType(ints) == &tloInt);
    assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloDArrayMoveBack(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetValueType(ints) == &tloInt);
    assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  TloError error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  tloDArrayPopBack(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &(int){(int)i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetValueType(ints) == &tloInt);
    assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

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
  assert(tloDArrayGetValueType(ints) == &tloInt);
  assert(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntConstructCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = malloc(sizeof(*copy));
  assert(copy);

  TloError error = tloDArrayConstructCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  assert(tloDArrayGetAllocatorType(ints) == tloDArrayGetAllocatorType(copy));

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = tloDArrayMakeCopy(ints);
  assert(copy);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  assert(tloDArrayGetAllocatorType(ints) == tloDArrayGetAllocatorType(copy));

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
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  TloDArray *copy = tloDArrayMake(&tloInt, &tloCountingAllocator);
  assert(copy);

  TloError error = tloDArrayCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  assert(tloDArrayGetAllocatorType(ints) == tloDArrayGetAllocatorType(copy));

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

static void testDArrayIntPtrPushBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator);
  assert(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  assert(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloDArrayPushBack(intPtrs, &intPtr);
  assert(!error);
  tloIntPtrDestruct(&intPtr);

  assert(tloDArrayGetSize(intPtrs) == 1);
  assert(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  assert(!tloDArrayIsEmpty(intPtrs));
  assert(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  assert(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  assert(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, 0))->ptr ==
         SOME_NUMBER);
  assert(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, 0))->ptr ==
         SOME_NUMBER);
  assert(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == SOME_NUMBER);
  assert(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == SOME_NUMBER);
  assert(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == SOME_NUMBER);
  assert(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == SOME_NUMBER);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResize(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator);
  assert(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  assert(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    assert(!error);

    assert(tloDArrayGetSize(intPtrs) == i + 1);
    assert(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    assert(!tloDArrayIsEmpty(intPtrs));
    assert(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    assert(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    assert(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
           (int)i);
    assert(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
           (int)i);
    assert(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
    assert(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
    assert(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == (int)i);
    assert(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == (int)i);
  }
  tloIntPtrDestruct(&intPtr);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackOncePopBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator);
  assert(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  assert(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloDArrayPushBack(intPtrs, &intPtr);
  assert(!error);
  tloIntPtrDestruct(&intPtr);

  tloDArrayPopBack(intPtrs);

  assert(tloDArrayGetSize(intPtrs) == 0);
  assert(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  assert(tloDArrayIsEmpty(intPtrs));
  assert(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  assert(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator);
  assert(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  assert(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    assert(!error);
  }
  tloIntPtrDestruct(&intPtr);

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloDArrayGetSize(intPtrs) == i + 1);
    assert(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    assert(!tloDArrayIsEmpty(intPtrs));
    assert(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    assert(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    assert(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
           (int)i);
    assert(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
           (int)i);
    assert(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
    assert(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
    assert(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == (int)i);
    assert(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == (int)i);

    tloDArrayPopBack(intPtrs);
  }

  assert(tloDArrayGetSize(intPtrs) == 0);
  assert(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  assert(tloDArrayIsEmpty(intPtrs));
  assert(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  assert(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
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
  testDArrayIntPtrPushBackOnce();
  testDArrayIntPtrPushBackUntilResize();
  testDArrayIntPtrPushBackOncePopBackOnce();
  testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty();

  assert(tloCountingAllocatorGetMallocCount() > 0);
  assert(tloCountingAllocatorGetMallocCount() ==
         tloCountingAllocatorGetFreeCount());
  assert(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloDArray): %zu\n", sizeof(TloDArray));
  printf("malloc count: %lu\nfree count: %lu\ntotal bytes allocated: %lu\n",
         tloCountingAllocatorGetMallocCount(),
         tloCountingAllocatorGetFreeCount(),
         tloCountingAllocatorGetTotalByteCount());
  puts("====================");
  puts("DArray tests passed.");
  puts("====================");
}
