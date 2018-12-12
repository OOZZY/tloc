#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "list_test_utils.h"
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
  testListHasFunctions((TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0),
                       TLO_LIST_CAPACITY | TLO_LIST_ELEMENT |
                           TLO_LIST_POP_BACK | TLO_LIST_UNORDERED_REMOVE);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), true);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), false);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), true);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0), false);
  testListIntPushBackOncePopBackOnce(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0));
  testListIntPushBackManyTimesPopBackUntilEmpty(
      (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0));
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
  testListIntPtrPushBackOncePopBackOnce(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0));
  testListIntPtrPushBackManyTimesPopBackUntilEmpty(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0));
  testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0));
  testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
      (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0));
  testDArrayFinalCounts();
  puts("==================");
  puts("DArray tests done.");
  puts("==================");
}
