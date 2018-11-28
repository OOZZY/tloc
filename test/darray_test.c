#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "tloc_test.h"
#include "util.h"

static void testDArrayInitialCounts(void) {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

#define EXPECT_DARRAY_PROPERTIES(darray, size, isEmpty, valueType,          \
                                 allocatorType)                             \
  do {                                                                      \
    TLO_EXPECT(tlovListSize(&(darray)->list) == (size));                    \
    TLO_EXPECT(tloDArrayCapacity(darray) >= tlovListSize(&(darray)->list)); \
    TLO_EXPECT(tlovListIsEmpty(&(darray)->list) == (isEmpty));              \
    TLO_EXPECT(tloListValueType(&(darray)->list) == (valueType));           \
    TLO_EXPECT(tloListAllocatorType(&(darray)->list) == (allocatorType));   \
  } while (0)

static void testDArrayIntConstructDestruct(void) {
  TloDArray ints;

  TloError error = tloDArrayConstruct(&ints, &tloInt, &countingAllocator, 0);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(&ints, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

#define EXPECT_DARRAY_ALL_PROPERTIES(darray, size, capacity, isEmpty,          \
                                     valueType, allocatorType)                 \
  do {                                                                         \
    EXPECT_DARRAY_PROPERTIES(darray, size, isEmpty, valueType, allocatorType); \
    TLO_EXPECT(tloDArrayCapacity(darray) == (capacity));                       \
  } while (0)

static void testDArrayIntConstructWithCapacityDestruct(void) {
  TloDArray ints;

  TloError error =
      tloDArrayConstruct(&ints, &tloInt, &countingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(&ints, 0, SOME_NUMBER, true, &tloInt,
                               &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, SOME_NUMBER);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, SOME_NUMBER, true, &tloInt,
                               &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayDeleteWithNull(void) { tloListDelete(NULL); }

#define EXPECT_DARRAY_INT_ELEMENTS(darray, index, indexValue, frontValue,      \
                                   backValue)                                  \
  do {                                                                         \
    TLO_EXPECT(*(const int *)tloDArrayElement(darray, index) == (indexValue)); \
    TLO_EXPECT(*(int *)tloDArrayMutableElement(darray, index) ==               \
               (indexValue));                                                  \
    TLO_EXPECT(*(const int *)tlovListFront(&(darray)->list) == (frontValue));  \
    TLO_EXPECT(*(int *)tlovListMutableFront(&(darray)->list) == (frontValue)); \
    TLO_EXPECT(*(const int *)tlovListBack(&(darray)->list) == (backValue));    \
    TLO_EXPECT(*(int *)tlovListMutableBack(&(darray)->list) == (backValue));   \
  } while (0)

static void testDArrayIntPushOrMoveBackOnce(bool testPush) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  TloError error;
  if (testPush) {
    int value = SOME_NUMBER;
    error = tlovListPushBack(&ints->list, &value);
  } else {
    int *value = makeInt(SOME_NUMBER);
    TLO_ASSERT(value);
    error = tlovListMoveBack(&ints->list, value);
  }
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_DARRAY_INT_ELEMENTS(ints, 0, SOME_NUMBER, SOME_NUMBER, SOME_NUMBER);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntPushOrMoveBackUntilResize(bool testPush) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      int value = (int)i;
      error = tlovListPushBack(&ints->list, &value);
    } else {
      int *value = makeInt((int)i);
      TLO_ASSERT(value);
      error = tlovListMoveBack(&ints->list, value);
    }
    TLO_ASSERT(!error);

    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, (int)i, 0, (int)i);
  }

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntPushBackOncePopBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tlovListPushBack(&ints->list, &value);
  TLO_ASSERT(!error);

  tloDArrayPopBack(ints);

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tlovListPushBack(&ints->list, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, (int)i, 0, (int)i);

    tloDArrayPopBack(ints);
  }

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntConstructCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloDArrayConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tloDArrayCapacity(copy),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tloDArrayElement(ints, i);
    const int *elem2 = tloDArrayElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  ints = NULL;

  tlovListDestruct(&copy->list);
  free(copy);
  copy = NULL;
}

static void testDArrayIntMakeCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = tloDArrayMakeCopy(ints);
  TLO_ASSERT(copy);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tloDArrayCapacity(copy),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tloDArrayElement(ints, i);
    const int *elem2 = tloDArrayElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  ints = NULL;

  tloListDelete(&copy->list);
  copy = NULL;
}

static void testDArrayIntCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(copy);

  TloError error = tloDArrayCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tloDArrayCapacity(copy),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tloDArrayElement(ints, i);
    const int *elem2 = tloDArrayElement(copy, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  ints = NULL;

  tloListDelete(&copy->list);
  copy = NULL;
}

#define EXPECT_DARRAY_INTPTR_ELEMENTS(darray, index, indexValue, frontValue, \
                                      backValue)                             \
  do {                                                                       \
    TLO_EXPECT(*((const IntPtr *)tloDArrayElement(darray, index))->ptr ==    \
               (indexValue));                                                \
    TLO_EXPECT(*((IntPtr *)tloDArrayMutableElement(darray, index))->ptr ==   \
               (indexValue));                                                \
    TLO_EXPECT(*((const IntPtr *)tlovListFront(&(darray)->list))->ptr ==     \
               (frontValue));                                                \
    TLO_EXPECT(*((IntPtr *)tlovListMutableFront(&(darray)->list))->ptr ==    \
               (frontValue));                                                \
    TLO_EXPECT(*((const IntPtr *)tlovListBack(&(darray)->list))->ptr ==      \
               (backValue));                                                 \
    TLO_EXPECT(*((IntPtr *)tlovListMutableBack(&(darray)->list))->ptr ==     \
               (backValue));                                                 \
  } while (0)

static void testDArrayIntPtrPushOrMoveBackOnce(bool testPush) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  TloError error;
  if (testPush) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, SOME_NUMBER);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(SOME_NUMBER);
    TLO_ASSERT(intPtr);
    error = tlovListMoveBack(&intPtrs->list, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_DARRAY_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, 0, SOME_NUMBER, SOME_NUMBER,
                                SOME_NUMBER);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushOrMoveBackUntilResize(bool testPush) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      IntPtr intPtr;
      error = intPtrConstruct(&intPtr, (int)i);
      TLO_ASSERT(!error);
      error = tlovListPushBack(&intPtrs->list, &intPtr);
      tloPtrDestruct(&intPtr);
    } else {
      IntPtr *intPtr = intPtrMake((int)i);
      TLO_ASSERT(intPtr);
      error = tlovListMoveBack(&intPtrs->list, intPtr);
    }
    TLO_ASSERT(!error);

    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);
  }

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackOncePopBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, SOME_NUMBER);
  TLO_ASSERT(!error);
  error = tlovListPushBack(&intPtrs->list, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tloDArrayPopBack(intPtrs);

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);

    tloDArrayPopBack(intPtrs);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, (int)i, 0, (int)i);

    tloDArrayUnorderedRemove(intPtrs, tlovListSize(&intPtrs->list) - 1);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);

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

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloDArray): %zu\n", sizeof(TloDArray));
  countingAllocatorPrintCounts();
}

void testDArray(void) {
  testDArrayInitialCounts();
  testDArrayIntConstructDestruct();
  testDArrayIntConstructWithCapacityDestruct();
  testDArrayIntMakeDelete();
  testDArrayIntMakeWithCapacityDelete();
  testDArrayDeleteWithNull();
  testDArrayIntPushOrMoveBackOnce(true);
  testDArrayIntPushOrMoveBackOnce(false);
  testDArrayIntPushOrMoveBackUntilResize(true);
  testDArrayIntPushOrMoveBackUntilResize(false);
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntConstructCopy();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();
  testDArrayIntPtrPushOrMoveBackOnce(true);
  testDArrayIntPtrPushOrMoveBackOnce(false);
  testDArrayIntPtrPushOrMoveBackUntilResize(true);
  testDArrayIntPtrPushOrMoveBackUntilResize(false);
  testDArrayIntPtrPushBackOncePopBackOnce();
  testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty();
  testDArrayFinalCounts();
  puts("==================");
  puts("DArray tests done.");
  puts("==================");
}
