#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "tloc_test.h"

#define EXPECT_DARRAY_PROPERTIES(darray, size, isEmpty, valueType,        \
                                 allocatorType)                           \
  do {                                                                    \
    TLO_EXPECT(tloDArrayGetSize(darray) == (size));                       \
    TLO_EXPECT(tloDArrayGetCapacity(darray) >= tloDArrayGetSize(darray)); \
    TLO_EXPECT(tloDArrayIsEmpty(darray) == (isEmpty));                    \
    TLO_EXPECT(tloDArrayGetValueType(darray) == (valueType));             \
    TLO_EXPECT(tloDArrayGetAllocatorType(darray) == (allocatorType));     \
  } while (0)

static void testDArrayIntConstructDestructStackSpace(void) {
  TloDArray ints;

  TloError error = tloDArrayConstruct(&ints, &tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(&ints, 0, true, &tloInt, &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error = tloDArrayConstruct(ints, &tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

#define EXPECT_DARRAY_ALL_PROPERTIES(darray, size, capacity, isEmpty,          \
                                     valueType, allocatorType)                 \
  do {                                                                         \
    EXPECT_DARRAY_PROPERTIES(darray, size, isEmpty, valueType, allocatorType); \
    TLO_EXPECT(tloDArrayGetCapacity(darray) == (capacity));                    \
  } while (0)

static void testDArrayIntConstructWithCapacityDestructStackSpace(void) {
  TloDArray ints;

  TloError error =
      tloDArrayConstruct(&ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(&ints, 0, SOME_NUMBER, true, &tloInt,
                               &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

static void testDArrayIntConstructWithCapacityDestructHeapSpace(void) {
  TloDArray *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error =
      tloDArrayConstruct(ints, &tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, SOME_NUMBER, true, &tloInt,
                               &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, SOME_NUMBER);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, SOME_NUMBER, true, &tloInt,
                               &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayDestructWithNull(void) { tloDArrayDestruct(NULL); }

static void testDArrayDeleteWithNull(void) { tloDArrayDelete(NULL); }

#define EXPECT_DARRAY_INT_ELEMENTS(darray, index, indexValue, frontValue, \
                                   backValue)                             \
  do {                                                                    \
    TLO_EXPECT(*(const int *)tloDArrayGetElement(darray, index) ==        \
               (indexValue));                                             \
    TLO_EXPECT(*(int *)tloDArrayGetMutableElement(darray, index) ==       \
               (indexValue));                                             \
    TLO_EXPECT(*(const int *)tloDArrayGetFront(darray) == (frontValue));  \
    TLO_EXPECT(*(int *)tloDArrayGetMutableFront(darray) == (frontValue)); \
    TLO_EXPECT(*(const int *)tloDArrayGetBack(darray) == (backValue));    \
    TLO_EXPECT(*(int *)tloDArrayGetMutableBack(darray) == (backValue));   \
  } while (0)

static void testDArrayIntPushBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  TloError error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_DARRAY_INT_ELEMENTS(ints, 0, SOME_NUMBER, SOME_NUMBER, SOME_NUMBER);

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
  EXPECT_DARRAY_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_DARRAY_INT_ELEMENTS(ints, 0, SOME_NUMBER, SOME_NUMBER, SOME_NUMBER);

  tloDArrayDelete(ints);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResize(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &tloCountingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloDArrayPushBack(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, (int)i, 0, (int)i);
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
    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, (int)i, 0, (int)i);
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

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

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
    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, (int)i, 0, (int)i);

    tloDArrayPopBack(ints);
  }

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

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

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tloDArrayGetSize(copy), tloDArrayGetCapacity(copy),
      tloDArrayIsEmpty(copy), tloDArrayGetValueType(copy),
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

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tloDArrayGetSize(copy), tloDArrayGetCapacity(copy),
      tloDArrayIsEmpty(copy), tloDArrayGetValueType(copy),
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

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tloDArrayGetSize(copy), tloDArrayGetCapacity(copy),
      tloDArrayIsEmpty(copy), tloDArrayGetValueType(copy),
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

#define EXPECT_DARRAY_INTPTR_ELEMENTS(darray, index, indexValue, frontValue, \
                                      backValue)                             \
  do {                                                                       \
    TLO_EXPECT(                                                              \
        *((const TloIntPtr *)tloDArrayGetElement(darray, index))->ptr ==     \
        (indexValue));                                                       \
    TLO_EXPECT(                                                              \
        *((TloIntPtr *)tloDArrayGetMutableElement(darray, index))->ptr ==    \
        (indexValue));                                                       \
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetFront(darray))->ptr ==       \
               (frontValue));                                                \
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableFront(darray))->ptr ==      \
               (frontValue));                                                \
    TLO_EXPECT(*((const TloIntPtr *)tloDArrayGetBack(darray))->ptr ==        \
               (backValue));                                                 \
    TLO_EXPECT(*((TloIntPtr *)tloDArrayGetMutableBack(darray))->ptr ==       \
               (backValue));                                                 \
  } while (0)

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

  EXPECT_DARRAY_PROPERTIES(intPtrs, 1, false, &tloIntPtr,
                           &tloCountingAllocator);
  EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, 0, SOME_NUMBER, SOME_NUMBER,
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

    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);
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

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);

    tloDArrayPopBack(intPtrs);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);

    tloDArrayUnorderedRemove(intPtrs, tloDArrayGetSize(intPtrs) - 1);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);

    if (i == SOME_NUMBER - 1) {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, SOME_NUMBER - 1, 0,
                                    SOME_NUMBER - 1);
    } else if (i == 0) {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, 1, 1, 1);
    } else {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, (int)i + 1, (int)i);
    }

    tloDArrayUnorderedRemove(intPtrs, 0);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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
