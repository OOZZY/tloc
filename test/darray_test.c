#include "darray_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <tlo/darray.h>
#include <tlo/test.h>
#include "list_test_utils.h"
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
                                     _valueType, _allocator)              \
  do {                                                                    \
    EXPECT_LIST_PROPERTIES(&(_darray)->list, _size, _isEmpty, _valueType, \
                           _allocator);                                   \
    TLO_EXPECT(tlovListCapacity(&(_darray)->list) == (_capacity));        \
  } while (0)

static void testDArrayIntConstructWithCapacityDestruct(void) {
  TloDArray ints;

  TloError error =
      tloDArrayConstruct(&ints, &tloInt, &countingAllocator, MAX_LIST_SIZE);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(&ints, 0, MAX_LIST_SIZE, true, &tloInt,
                               &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testDArrayIntMakeDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
}

static void testDArrayIntMakeWithCapacityDelete(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, MAX_LIST_SIZE);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, MAX_LIST_SIZE, true, &tloInt,
                               &countingAllocator);

  tloListDelete(&ints->list);
}

static void testDArrayIntConstructCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
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

static void testDArrayIntMakeCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDArray *copy = tloDArrayMakeCopy(ints);
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

static void testDArrayIntCopy(void) {
  TloDArray *ints = tloDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
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

static void testDArrayFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloDArray): %zu\n", sizeof(TloDArray));
  countingAllocatorPrintCounts();
}

static TloList *makeListInt(void) {
  return (TloList *)tloDArrayMake(&tloInt, &countingAllocator, 0);
}

static TloList *makeListIntPtr(void) {
  return (TloList *)tloDArrayMake(&intPtrType, &countingAllocator, 0);
}

void testDArray(void) {
  testDArrayInitialCounts();

  testDArrayIntConstructDestruct();
  testDArrayIntConstructWithCapacityDestruct();
  testDArrayIntMakeDelete();
  testDArrayIntMakeWithCapacityDelete();
  testDArrayIntConstructCopy();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();

  testListHasFunctions(makeListInt(), TLO_LIST_CAPACITY | TLO_LIST_ELEMENT |
                                          TLO_LIST_POP_BACK |
                                          TLO_LIST_UNORDERED_REMOVE);

  testListIntPushBackOnce(makeListInt(), true);
  testListIntPushBackOnce(makeListInt(), false);
  testListIntPushBackManyTimes(makeListInt(), true);
  testListIntPushBackManyTimes(makeListInt(), false);
  testListIntPushBackOncePopBackOnce(makeListInt());
  testListIntPushBackManyTimesPopBackUntilEmpty(makeListInt());

  testListIntPtrPushBackOnce(makeListIntPtr(), true);
  testListIntPtrPushBackOnce(makeListIntPtr(), false);
  testListIntPtrPushBackManyTimes(makeListIntPtr(), true);
  testListIntPtrPushBackManyTimes(makeListIntPtr(), false);
  testListIntPtrPushBackOncePopBackOnce(makeListIntPtr());
  testListIntPtrPushBackManyTimesPopBackUntilEmpty(makeListIntPtr());
  testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
      makeListIntPtr());
  testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
      makeListIntPtr());

  testDArrayFinalCounts();
  puts("==================");
  puts("DArray tests done.");
  puts("==================");
}
