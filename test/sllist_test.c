#include "sllist_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "list_test_utils.h"
#include "util.h"

static void testSLListInitialCounts() {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

static void testSLListIntConstructDestruct(void) {
  TloSLList ints;

  tloSLListConstruct(&ints, &tloInt, &countingAllocator);

  EXPECT_LIST_PROPERTIES(&ints.list, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
}

static void testSLListIntConstructCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloSLListConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloSLLNode *node1 = tloSLListHead(ints);
  const TloSLLNode *node2 = tloSLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeElement(node1);
    const int *elem2 = tloSLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloSLLNodeNext(node1);
    node2 = tloSLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tlovListDestruct(&copy->list);
  free(copy);
}

static void testSLListIntMakeCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMakeCopy(ints);
  TLO_ASSERT(copy);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloSLLNode *node1 = tloSLListHead(ints);
  const TloSLLNode *node2 = tloSLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeElement(node1);
    const int *elem2 = tloSLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloSLLNodeNext(node1);
    node2 = tloSLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testSLListIntCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(copy);

  TloError error = tloSLListCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloSLLNode *node1 = tloSLListHead(ints);
  const TloSLLNode *node2 = tloSLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeElement(node1);
    const int *elem2 = tloSLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloSLLNodeNext(node1);
    node2 = tloSLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testSLListFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloSLList): %zu\n", sizeof(TloSLList));
  printf("sizeof(TloSLLNode): %zu\n", sizeof(TloSLLNode));
  countingAllocatorPrintCounts();
}

static TloList *makeListInt(void) {
  return (TloList *)tloSLListMake(&tloInt, &countingAllocator);
}

static TloList *makeListIntPtr(void) {
  return (TloList *)tloSLListMake(&intPtrType, &countingAllocator);
}

void testSLList(void) {
  testSLListInitialCounts();

  testSLListIntConstructDestruct();
  testSLListIntMakeDelete();
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();

  testListHasFunctions(makeListInt(), TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT);

  testListIntPushOrMoveBackOnce(makeListInt(), true);
  testListIntPushOrMoveBackOnce(makeListInt(), false);
  testListIntPushOrMoveBackManyTimes(makeListInt(), true);
  testListIntPushOrMoveBackManyTimes(makeListInt(), false);
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
  testListIntPtrPushOrMoveFrontOnce(makeListIntPtr(), true);
  testListIntPtrPushOrMoveFrontOnce(makeListIntPtr(), false);
  testListIntPtrPushOrMoveFrontManyTimes(makeListIntPtr(), true);
  testListIntPtrPushOrMoveFrontManyTimes(makeListIntPtr(), false);
  testListIntPtrPushFrontOncePopFrontOnce(makeListIntPtr());
  testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(makeListIntPtr());

  testSLListFinalCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
