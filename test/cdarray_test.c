#include "cdarray_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <tlo/cdarray.h>
#include <tlo/test.h>
#include "list_test_utils.h"
#include "util.h"

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
      tloCDArrayConstruct(&ints, &tloInt, &countingAllocator, MAX_LIST_SIZE);
  TLO_ASSERT(!error);

  EXPECT_DARRAY_ALL_PROPERTIES(&ints, 0, MAX_LIST_SIZE, true, &tloInt,
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
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, MAX_LIST_SIZE);
  TLO_ASSERT(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, MAX_LIST_SIZE, true, &tloInt,
                               &countingAllocator);

  tloListDelete(&ints->list);
}

static void testCDArrayIntConstructCopy(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
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

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
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

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
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

static void testCDArrayIntShrinkWhenElementsWrapAround(void) {
  TloCDArray *ints = tloCDArrayMake(&tloInt, &countingAllocator, 0);
  TLO_ASSERT(ints);

  TloError error;

  for (int i = 0; i < 8; ++i) {
    error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  /*
       Front
       |
  [6 7 0 1 2 3 4 5]
  */
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 8, 8, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, 0, 7, 4, 4);

  for (int i = 0; i < 5; ++i) {
    tlovListPopFront(&ints->list);
  }

  /*
                 Front
                 |
  [6 7 - - - - - 5]
  */
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 3, 8, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, 5, 7, 1, 6);

  tlovListPopBack(&ints->list);

  /*
     Front
     |
  [- 5 6 -]
  */
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 2, 4, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, 5, 6, 1, 6);

  int value = 7;
  error = tlovListPushBack(&ints->list, &value);
  TLO_ASSERT(!error);

  /*
     Front
     |
  [- 5 6 7]
  */
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 3, 4, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, 5, 7, 1, 6);

  tlovListPopBack(&ints->list);

  /*
     Front
     |
  [- 5 6 -]
  */
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 2, 4, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, 5, 6, 0, 5);

  tloListDelete(&ints->list);
}

static TloList *makeListInt(void) {
  return (TloList *)tloCDArrayMake(&tloInt, &countingAllocator, 0);
}

static TloList *makeListIntPtr(void) {
  return (TloList *)tloCDArrayMake(&intPtrType, &countingAllocator, 0);
}

void testCDArray(void) {
  testInitialCounts();

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

  testListIntPushBackOnce(makeListInt(), true);
  testListIntPushBackOnce(makeListInt(), false);
  testListIntPushBackManyTimes(makeListInt(), true);
  testListIntPushBackManyTimes(makeListInt(), false);
  testListIntPushBackOncePopBackOnce(makeListInt());
  testListIntPushBackManyTimesPopBackUntilEmpty(makeListInt());
  testListIntPushFrontOnce(makeListInt(), true);
  testListIntPushFrontOnce(makeListInt(), false);
  testListIntPushFrontManyTimes(makeListInt(), true);
  testListIntPushFrontManyTimes(makeListInt(), false);
  testListIntPushFrontOncePopFrontOnce(makeListInt());
  testListIntPushFrontManyTimesPopFrontUntilEmpty(makeListInt());

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
  testListIntPtrPushFrontOnce(makeListIntPtr(), true);
  testListIntPtrPushFrontOnce(makeListIntPtr(), false);
  testListIntPtrPushFrontManyTimes(makeListIntPtr(), true);
  testListIntPtrPushFrontManyTimes(makeListIntPtr(), false);
  testListIntPtrPushFrontOncePopFrontOnce(makeListIntPtr());
  testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(makeListIntPtr());

  testCDArrayIntShrinkWhenElementsWrapAround();

  printf("sizeof(TloCDArray): %zu\n", sizeof(TloCDArray));
  testFinalCounts();
  puts("===================");
  puts("CDArray tests done.");
  puts("===================");
}
