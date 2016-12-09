#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "tloc_test.h"

static void testDArrayIntConstructDestructStackSpace(void) {
  TloDArray ints;

  TloError error = tloDArrayConstruct(&ints, &tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(&ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(&ints) >= tloDArrayGetSize(&ints));
  TLO_EXPECT(tloDArrayIsEmpty(&ints));
  TLO_EXPECT(tloDArrayGetValueType(&ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error = tloDArrayConstruct(ints, &tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntConstructWithCapacityDestructStackSpace(void) {
  TloDArray ints;

  TloError error =
      tloDArrayConstruct(&ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(&ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(&ints) == SOME_NUMBER);
  TLO_EXPECT(tloDArrayIsEmpty(&ints));
  TLO_EXPECT(tloDArrayGetValueType(&ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructWithCapacityDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error =
      tloDArrayConstruct(ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(ints);

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) == SOME_NUMBER);
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayDestructWithNull(void) { tloDArrayDestruct(NULL); }

static void testDArrayDeleteWithNull(void) { tloDArrayDelete(NULL); }

static void testDArrayIntPushBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  TloError error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(ints) == 1);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(!tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_EXPECT(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  TLO_EXPECT(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  TLO_EXPECT(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMoveBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloDArrayMoveBack(ints, &value);
  TLO_ASSERT(!error);

  TLO_EXPECT(value == 0);
  TLO_EXPECT(tloDArrayGetSize(ints) == 1);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(!tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_EXPECT(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  TLO_EXPECT(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  TLO_EXPECT(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  TLO_EXPECT(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResize(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    TLO_EXPECT(tloDArrayGetSize(ints) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    TLO_EXPECT(!tloDArrayIsEmpty(ints));
    TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
    TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_EXPECT(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    TLO_EXPECT(*(const int *)tloDArrayGetFront(ints) == 0);
    TLO_EXPECT(*(int *)tloDArrayGetMutableFront(ints) == 0);
    TLO_EXPECT(*(const int *)tloDArrayGetBack(ints) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableBack(ints) == (int)i);
  }

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMoveBackUntilResize(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloDArrayMoveBack(ints, &value);
    TLO_ASSERT(!error);

    TLO_EXPECT(value == 0);
    TLO_EXPECT(tloDArrayGetSize(ints) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    TLO_EXPECT(!tloDArrayIsEmpty(ints));
    TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
    TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_EXPECT(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    TLO_EXPECT(*(const int *)tloDArrayGetFront(ints) == 0);
    TLO_EXPECT(*(int *)tloDArrayGetMutableFront(ints) == 0);
    TLO_EXPECT(*(const int *)tloDArrayGetBack(ints) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableBack(ints) == (int)i);
  }

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackOncePopBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  TloError error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  tloDArrayPopBack(ints);

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &(int){(int)i});
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_EXPECT(tloDArrayGetSize(ints) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    TLO_EXPECT(!tloDArrayIsEmpty(ints));
    TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
    TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_EXPECT(*(const int *)tloDArrayGetElement(ints, i) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableElement(ints, i) == (int)i);
    TLO_EXPECT(*(const int *)tloDArrayGetFront(ints) == 0);
    TLO_EXPECT(*(int *)tloDArrayGetMutableFront(ints) == 0);
    TLO_EXPECT(*(const int *)tloDArrayGetBack(ints) == (int)i);
    TLO_EXPECT(*(int *)tloDArrayGetMutableBack(ints) == (int)i);

    tloDArrayPopBack(ints);
  }

  TLO_EXPECT(tloDArrayGetSize(ints) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  TLO_EXPECT(tloDArrayIsEmpty(ints));
  TLO_EXPECT(tloDArrayGetValueType(ints) == &tloInt);
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntConstructCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloDArrayConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  TLO_EXPECT(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  TLO_EXPECT(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  TLO_EXPECT(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) ==
             tloDArrayGetAllocatorType(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDestruct(copy);
  free(copy);
  copy = NULL;
}

static void testDArrayIntMakeCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = tloDArrayMakeCopy(ints);
  TLO_ASSERT(copy);

  TLO_EXPECT(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  TLO_EXPECT(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  TLO_EXPECT(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  TLO_EXPECT(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) ==
             tloDArrayGetAllocatorType(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
}

static void testDArrayIntCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(copy);

  TloError error = tloDArrayCopy(copy, ints);
  TLO_ASSERT(!error);

  TLO_EXPECT(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  TLO_EXPECT(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  TLO_EXPECT(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  TLO_EXPECT(tloDArrayGetValueType(ints) == tloDArrayGetValueType(copy));
  TLO_EXPECT(tloDArrayGetAllocatorType(ints) ==
             tloDArrayGetAllocatorType(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
}

static void testDArrayIntPtrPushBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloDArrayPushBack(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  TLO_EXPECT(tloDArrayGetSize(intPtrs) == 1);
  TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  TLO_EXPECT(!tloDArrayIsEmpty(intPtrs));
  TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, 0))->ptr ==
             SOME_NUMBER);
  TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, 0))->ptr ==
             SOME_NUMBER);
  TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr ==
             SOME_NUMBER);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResize(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);

    TLO_EXPECT(tloDArrayGetSize(intPtrs) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    TLO_EXPECT(!tloDArrayIsEmpty(intPtrs));
    TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == (int)i);
  }
  tloPtrDestruct(&intPtr);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackOncePopBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloDArrayPushBack(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tloDArrayPopBack(intPtrs);

  TLO_EXPECT(tloDArrayGetSize(intPtrs) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  TLO_EXPECT(tloDArrayIsEmpty(intPtrs));
  TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
  }
  tloPtrDestruct(&intPtr);

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_EXPECT(tloDArrayGetSize(intPtrs) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    TLO_EXPECT(!tloDArrayIsEmpty(intPtrs));
    TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == (int)i);

    tloDArrayPopBack(intPtrs);
  }

  TLO_EXPECT(tloDArrayGetSize(intPtrs) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  TLO_EXPECT(tloDArrayIsEmpty(intPtrs));
  TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
  }
  tloPtrDestruct(&intPtr);

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_EXPECT(tloDArrayGetSize(intPtrs) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    TLO_EXPECT(!tloDArrayIsEmpty(intPtrs));
    TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
               (int)i);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == (int)i);
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == (int)i);

    tloDArrayUnorderedRemove(intPtrs, tloDArrayGetSize(intPtrs) - 1);
  }

  TLO_EXPECT(tloDArrayGetSize(intPtrs) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  TLO_EXPECT(tloDArrayIsEmpty(intPtrs));
  TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&tloIntPtr, &tloCountingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloDArrayPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
  }
  tloPtrDestruct(&intPtr);

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_EXPECT(tloDArrayGetSize(intPtrs) == i + 1);
    TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
    TLO_EXPECT(!tloDArrayIsEmpty(intPtrs));
    TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
    TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

    if (i == SOME_NUMBER - 1) {
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
                 SOME_NUMBER - 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
                 SOME_NUMBER - 1);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 0);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 0);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr ==
                 SOME_NUMBER - 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr ==
                 SOME_NUMBER - 1);
    } else if (i == 0) {
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
                 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
                 1);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr == 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr == 1);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr == 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr == 1);
    } else {
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetElement(intPtrs, i))->ptr ==
                 (int)i);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableElement(intPtrs, i))->ptr ==
                 (int)i);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(intPtrs))->ptr ==
                 (int)i + 1);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(intPtrs))->ptr ==
                 (int)i + 1);
      TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(intPtrs))->ptr ==
                 (int)i);
      TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(intPtrs))->ptr ==
                 (int)i);
    }

    tloDArrayUnorderedRemove(intPtrs, 0);
  }

  TLO_EXPECT(tloDArrayGetSize(intPtrs) == 0);
  TLO_EXPECT(tloDArrayGetCapacity(intPtrs) >= tloDArrayGetSize(intPtrs));
  TLO_EXPECT(tloDArrayIsEmpty(intPtrs));
  TLO_EXPECT(tloDArrayGetValueType(intPtrs) == &tloIntPtr);
  TLO_EXPECT(tloDArrayGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloDArrayDelete(intPtrs);
  intPtrs = NULL;
}

void testDArray(void) {
  tloCountingAllocatorResetCounts();
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() == 0);
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_EXPECT(tloCountingAllocatorGetTotalByteCount() == 0);

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
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty();

  TLO_EXPECT(tloCountingAllocatorGetMallocCount() > 0);
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_EXPECT(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloDArray): %zu\n", sizeof(TloDArray));
  tloCountingAllocatorPrintCounts();
  puts("==================");
  puts("DArray tests done.");
  puts("==================");
}
