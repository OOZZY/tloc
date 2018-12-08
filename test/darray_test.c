#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "list_test.h"
#include "tloc_test.h"
#include "util.h"

static void testDArrayInitialCounts(void) {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

static void testDArrayIntConstructDestruct(void) {
  TloDArray ints;

  TloError error = tloDArrayConstruct(&ints, &tloInt, &countingAllocator, 0);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(&ints.list, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

#define EXPECT_DARRAY_ALL_PROPERTIES(_darray, _size, _capacity, _isEmpty, \
                                     _valueType, _allocatorType)          \
  do {                                                                    \
    EXPECT_LIST_PROPERTIES(&(_darray)->list, _size, _isEmpty, _valueType, \
                           _allocatorType);                               \
    TLO_EXPECT(tlovListCapacity(&(_darray)->list) == (_capacity));        \
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

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

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

static void testDArrayIntPushBackOncePopBackOnce(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(&ints->list, TLO_LIST_POP_BACK));

  int value = SOME_NUMBER;
  TloError error = tlovListPushBack(&ints->list, &value);
  TLO_ASSERT(!error);

  tlovListPopBack(&ints->list);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(&ints->list, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tlovListPushBack(&ints->list, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&ints->list, i + 1, false, &tloInt,
                           &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(&ints->list, 0, (int)i, i, (int)i);

    tlovListPopBack(&ints->list);
  }

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

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
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
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
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
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
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocatorType(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  ints = NULL;

  tloListDelete(&copy->list);
  copy = NULL;
}

static void testDArrayIntPtrPushBackOncePopBackOnce(void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(&intPtrs->list, TLO_LIST_POP_BACK));

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, SOME_NUMBER);
  TLO_ASSERT(!error);
  error = tlovListPushBack(&intPtrs->list, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tlovListPopBack(&intPtrs->list);

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty(void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(&intPtrs->list, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&intPtrs->list, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, 0, (int)i, i, (int)i);

    tlovListPopBack(&intPtrs->list);
  }

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(&intPtrs->list, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&intPtrs->list, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, 0, (int)i, i, (int)i);

    tlovListUnorderedRemove(&intPtrs->list, tlovListSize(&intPtrs->list) - 1);
  }

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty(
    void) {
  TloDArray *intPtrs = tloDArrayMake(&intPtrType, &countingAllocator, 0);
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(&intPtrs->list, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(&intPtrs->list, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&intPtrs->list, i + 1, false, &intPtrType,
                           &countingAllocator);

    if (i == SOME_NUMBER - 1) {
      EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, 0, SOME_NUMBER - 1, i,
                                  SOME_NUMBER - 1);
    } else if (i == 0) {
      EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, 1, 1, i, 1);
    } else {
      EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, (int)i + 1, (int)i, i,
                                  (int)i);
    }

    tlovListUnorderedRemove(&intPtrs->list, 0);
  }

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

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
  testListIntPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), true);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), false);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), true);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), false);
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntConstructCopy();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0), true);
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0), false);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0), true);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0), false);
  testDArrayIntPtrPushBackOncePopBackOnce();
  testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty();
  testDArrayFinalCounts();
  puts("==================");
  puts("DArray tests done.");
  puts("==================");
}
