#include "cdarray_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <tlo/cdarray.h>
#include <tlo/test.h>
#include "list_test_utils.h"
#include "util.h"

static void testCDArrayInitialCounts(void) {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

static void testCDArrayIntConstructDestruct(void) {
  TloCDArray ints;

  TloError error = tloCDArrayConstruct(&ints, &tloInt, &countingAllocator, 0);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(&ints.list, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

#define EXPECT_DARRAY_ALL_PROPERTIES(_darray, _size, _capacity, _isEmpty, \
                                     _valueType, _allocator)              \
  do {                                                                    \
    EXPECT_LIST_PROPERTIES(&(_darray)->list, _size, _isEmpty, _valueType, \
                           _allocator);                                   \
    TLO_EXPECT(tlovListCapacity(&(_darray)->list) == (_capacity));        \
  } while (0)

static void testCDArrayIntConstructWithCapacityDestruct(void) {
  TloCDArray ints;

  TloError error =
      tloCDArrayConstruct(&ints, &tloInt, &countingAllocator, SOME_NUMBER);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(&ints, 0, SOME_NUMBER, true, &tloInt,
                               &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testCDArrayIntMakeDelete(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
}

static void testCDArrayIntMakeWithCapacityDelete(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, SOME_NUMBER);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, SOME_NUMBER, true, &tloInt,
                               &countingAllocator);

  tloListDelete(&ints->list);
}

static void testCDArrayIntConstructCopy(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloCDArray *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloCDArrayConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocator(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  tlovListDestruct(&copy->list);
  free(copy);
}

static void testCDArrayIntMakeCopy(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloCDArray *copy = tloCDArrayMakeCopy(ints);
  TLO_ASSERT(copy);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocator(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testCDArrayIntCopy(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloCDArray *copy = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(copy);

  TloError error = tloCDArrayCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(
      ints, tlovListSize(&copy->list), tlovListCapacity(&copy->list),
      tlovListIsEmpty(&copy->list), tloListValueType(&copy->list),
      tloListAllocator(&copy->list));

  for (size_t i = 0; i < tlovListSize(&ints->list); ++i) {
    const int *elem1 = tlovListElement(&ints->list, i);
    const int *elem2 = tlovListElement(&copy->list, i);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testCDArrayFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloCDArray): %zu\n", sizeof(TloCDArray));
  countingAllocatorPrintCounts();
}

static TloList *makeListInt(void) {
  return (TloList *)tloCDArrayMake(&tloInt, &countingAllocator, 0);
}

static TloList *makeListIntPtr(void) {
  return (TloList *)tloCDArrayMake(&intPtrType, &countingAllocator, 0);
}

void testCDArray(void) {
  testCDArrayInitialCounts();

  testCDArrayIntConstructDestruct();
  testCDArrayIntConstructWithCapacityDestruct();
  testCDArrayIntMakeDelete();
  testCDArrayIntMakeWithCapacityDelete();
  testCDArrayIntConstructCopy();
  testCDArrayIntMakeCopy();
  testCDArrayIntCopy();

  testListHasFunctions(makeListInt(),
                       TLO_LIST_CAPACITY | TLO_LIST_ELEMENT |
                           TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT |
                           TLO_LIST_POP_BACK | TLO_LIST_UNORDERED_REMOVE);

  testListIntPushOrMoveBackOnce(makeListInt(), true);
  testListIntPushOrMoveBackOnce(makeListInt(), false);
  testListIntPushOrMoveBackManyTimes(makeListInt(), true);
  testListIntPushOrMoveBackManyTimes(makeListInt(), false);
  testListIntPushBackOncePopBackOnce(makeListInt());
  testListIntPushBackManyTimesPopBackUntilEmpty(makeListInt());
  testListIntPushOrMoveFrontOnce(makeListInt(), true);
  testListIntPushOrMoveFrontOnce(makeListInt(), false);
  testListIntPushOrMoveFrontManyTimes(makeListInt(), true);
  testListIntPushOrMoveFrontManyTimes(makeListInt(), false);
  testListIntPushFrontOncePopFrontOnce(makeListInt());
  testListIntPushFrontManyTimesPopFrontUntilEmpty(makeListInt());

  testListIntPtrPushOrMoveBackOnce(makeListIntPtr(), true);
  testListIntPtrPushOrMoveBackOnce(makeListIntPtr(), false);
  testListIntPtrPushOrMoveBackManyTimes(makeListIntPtr(), true);
  testListIntPtrPushOrMoveBackManyTimes(makeListIntPtr(), false);
  testListIntPtrPushBackOncePopBackOnce(makeListIntPtr());
  testListIntPtrPushBackManyTimesPopBackUntilEmpty(makeListIntPtr());
  testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
      makeListIntPtr());
  testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
      makeListIntPtr());
  testListIntPtrPushOrMoveFrontOnce(makeListIntPtr(), true);
  testListIntPtrPushOrMoveFrontOnce(makeListIntPtr(), false);
  testListIntPtrPushOrMoveFrontManyTimes(makeListIntPtr(), true);
  testListIntPtrPushOrMoveFrontManyTimes(makeListIntPtr(), false);
  testListIntPtrPushFrontOncePopFrontOnce(makeListIntPtr());
  testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(makeListIntPtr());

  testCDArrayFinalCounts();
  puts("===================");
  puts("CDArray tests done.");
  puts("===================");
}
