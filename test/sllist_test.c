#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "list_test.h"
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

static void testSLListIntPushOrMoveFrontOnce(bool testPush) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  TloError error;
  if (testPush) {
    int value = SOME_NUMBER;
    error = tloSLListPushFront(ints, &value);
  } else {
    int *value = makeInt(SOME_NUMBER);
    TLO_ASSERT(value);
    error = tloSLListMoveFront(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(&ints->list, 1, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(&ints->list, SOME_NUMBER, SOME_NUMBER, 0,
                           SOME_NUMBER);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testSLListIntPushOrMoveFrontManyTimes(bool testPush) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      int value = (int)i;
      error = tloSLListPushFront(ints, &value);
    } else {
      int *value = makeInt((int)i);
      TLO_ASSERT(value);
      error = tloSLListMoveFront(ints, value);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(&ints->list, i + 1, false, &tloInt,
                           &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(&ints->list, (int)i, 0, i, 0);
  }

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testSLListIntPushFrontOncePopFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListPushFront(ints, &value);
  TLO_ASSERT(!error);

  tloSLListPopFront(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloSLListPushFront(ints, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&ints->list, i + 1, false, &tloInt,
                           &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(&ints->list, (int)i, 0, i, 0);

    tloSLListPopFront(ints);
  }

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
  ints = NULL;
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

static void testSLListIntPtrPushOrMoveFrontOnce(bool testPush) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  TloError error;
  if (testPush) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, SOME_NUMBER);
    TLO_ASSERT(!error);
    error = tloSLListPushFront(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(SOME_NUMBER);
    TLO_ASSERT(intPtr);
    error = tloSLListMoveFront(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 1, false, &intPtrType,
                         &countingAllocator);
  EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, SOME_NUMBER, SOME_NUMBER, 0,
                              SOME_NUMBER);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testSLListIntPtrPushOrMoveFrontManyTimes(bool testPush) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      IntPtr intPtr;
      error = intPtrConstruct(&intPtr, (int)i);
      TLO_ASSERT(!error);
      error = tloSLListPushFront(intPtrs, &intPtr);
      tloPtrDestruct(&intPtr);
    } else {
      IntPtr *intPtr = intPtrMake((int)i);
      TLO_ASSERT(intPtr);
      error = tloSLListMoveFront(intPtrs, intPtr);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(&intPtrs->list, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, (int)i, 0, i, 0);
  }

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testSLListIntPtrPushFrontOncePopFrontOnce(void) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, SOME_NUMBER);
  TLO_ASSERT(!error);
  error = tloSLListPushFront(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tloSLListPopFront(intPtrs);

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
}

static void testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tloSLListPushFront(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(&intPtrs->list, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(&intPtrs->list, (int)i, 0, i, 0);

    tloSLListPopFront(intPtrs);
  }

  EXPECT_LIST_PROPERTIES(&intPtrs->list, 0, true, &intPtrType,
                         &countingAllocator);

  tloListDelete(&intPtrs->list);
  intPtrs = NULL;
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
  testSLListIntPushOrMoveFrontOnce(true);
  testSLListIntPushOrMoveFrontOnce(false);
  testSLListIntPushOrMoveFrontManyTimes(true);
  testSLListIntPushOrMoveFrontManyTimes(false);
  testSLListIntPushFrontOncePopFrontOnce();
  testSLListIntPushFrontManyTimesPopFrontUntilEmpty();
  testListIntPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), true);
  testListIntPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&tloInt, &countingAllocator), false);
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();
  testSLListIntPtrPushOrMoveFrontOnce(true);
  testSLListIntPtrPushOrMoveFrontOnce(false);
  testSLListIntPtrPushOrMoveFrontManyTimes(true);
  testSLListIntPtrPushOrMoveFrontManyTimes(false);
  testSLListIntPtrPushFrontOncePopFrontOnce();
  testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty();
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveBackOnce(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), true);
  testListIntPtrPushOrMoveBackManyTimes(
      (TloList *)tloSLListMake(&intPtrType, &countingAllocator), false);
  testSLListFinalCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
