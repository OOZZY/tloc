#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "list_test_utils.h"
#include "tloc_test.h"
#include "util.h"

static void testSLListInitialCounts() {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

static void testSLListIntConstructDestruct(void) {
  TloSLList ints;

  TloError error = tloSLListConstruct(&ints, &tloInt, &countingAllocator);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(&ints.list, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testSLListDeleteWithNull(void) { tloListDelete(NULL); }

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
      tloListValueType(&copy->list), tloListAllocatorType(&copy->list));

  const TloSLLNode *node1 = tloSLLNodeHead(ints);
  const TloSLLNode *node2 = tloSLLNodeHead(copy);
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
  ints = NULL;

  tlovListDestruct(&copy->list);
  free(copy);
  copy = NULL;
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
      tloListValueType(&copy->list), tloListAllocatorType(&copy->list));

  const TloSLLNode *node1 = tloSLLNodeHead(ints);
  const TloSLLNode *node2 = tloSLLNodeHead(copy);
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
  ints = NULL;

  tloListDelete(&copy->list);
  copy = NULL;
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
      tloListValueType(&copy->list), tloListAllocatorType(&copy->list));

  const TloSLLNode *node1 = tloSLLNodeHead(ints);
  const TloSLLNode *node2 = tloSLLNodeHead(copy);
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
  ints = NULL;

  tloListDelete(&copy->list);
  copy = NULL;
}

static void testSLListFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloSLList): %zu\n", sizeof(TloSLList));
  printf("sizeof(TloSLLNode): %zu\n", sizeof(TloSLLNode));
  countingAllocatorPrintCounts();
}

void testSLList(void) {
  testSLListInitialCounts();
  testSLListIntConstructDestruct();
  testSLListIntMakeDelete();
  testSLListDeleteWithNull();
  testListHasFunctions((TloList *)tloSLListMake(&tloInt, &countingAllocator),
                       TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testListIntPushOrMoveFrontOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveFrontOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testListIntPushOrMoveFrontManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveFrontManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testListIntPushFrontOncePopFrontOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator));
  testListIntPushFrontManyTimesPopFrontUntilEmpty(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator));
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testListIntPtrPushOrMoveFrontOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveFrontOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testListIntPtrPushOrMoveFrontManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveFrontManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testListIntPtrPushFrontOncePopFrontOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator));
  testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator));
  testSLListFinalCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
