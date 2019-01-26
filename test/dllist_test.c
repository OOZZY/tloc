#include "dllist_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <tlo/dllist.h>
#include <tlo/test.h>
#include "list_test_utils.h"
#include "util.h"

static void testDLListInitialCounts() {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

static void testDLListIntConstructDestruct(void) {
  TloDLList ints;

  tloDLListConstruct(&ints, &tloInt, &countingAllocator);

  EXPECT_LIST_PROPERTIES(&ints.list, 0, true, &tloInt, &countingAllocator);

  tlovListDestruct(&ints.list);
}

static void testDLListIntMakeDelete(void) {
  TloDLList *ints = tloDLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  EXPECT_LIST_PROPERTIES(&ints->list, 0, true, &tloInt, &countingAllocator);

  tloListDelete(&ints->list);
}

static void testDLListIntConstructCopy(void) {
  TloDLList *ints = tloDLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDLList *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloDLListConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloDLLNode *node1 = tloDLListHead(ints);
  const TloDLLNode *node2 = tloDLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloDLLNodeElement(node1);
    const int *elem2 = tloDLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloDLLNodeNext(node1);
    node2 = tloDLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tlovListDestruct(&copy->list);
  free(copy);
}

static void testDLListIntMakeCopy(void) {
  TloDLList *ints = tloDLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDLList *copy = tloDLListMakeCopy(ints);
  TLO_ASSERT(copy);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloDLLNode *node1 = tloDLListHead(ints);
  const TloDLLNode *node2 = tloDLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloDLLNodeElement(node1);
    const int *elem2 = tloDLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloDLLNodeNext(node1);
    node2 = tloDLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testDLListIntCopy(void) {
  TloDLList *ints = tloDLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error = tlovListPushBack(&ints->list, &i);
    TLO_ASSERT(!error);
  }

  TloDLList *copy = tloDLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(copy);

  TloError error = tloDLListCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(
      &ints->list, tlovListSize(&copy->list), tlovListIsEmpty(&copy->list),
      tloListValueType(&copy->list), tloListAllocator(&copy->list));

  const TloDLLNode *node1 = tloDLListHead(ints);
  const TloDLLNode *node2 = tloDLListHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloDLLNodeElement(node1);
    const int *elem2 = tloDLLNodeElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloDLLNodeNext(node1);
    node2 = tloDLLNodeNext(node2);
  }

  tloListDelete(&ints->list);
  tloListDelete(&copy->list);
}

static void testDLListFinalCounts() {
  TLO_EXPECT(countingAllocatorMallocCount() > 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() > 0);

  printf("sizeof(TloDLList): %zu\n", sizeof(TloDLList));
  printf("sizeof(TloDLLNode): %zu\n", sizeof(TloDLLNode));
  countingAllocatorPrintCounts();
}

static TloList *makeListInt(void) {
  return (TloList *)tloDLListMake(&tloInt, &countingAllocator);
}

static TloList *makeListIntPtr(void) {
  return (TloList *)tloDLListMake(&intPtrType, &countingAllocator);
}

void testDLList(void) {
  testDLListInitialCounts();

  testDLListIntConstructDestruct();
  testDLListIntMakeDelete();
  testDLListIntConstructCopy();
  testDLListIntMakeCopy();
  testDLListIntCopy();

  testListHasFunctions(makeListInt(), TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT |
                                          TLO_LIST_POP_BACK);

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
  testListIntPtrPushFrontOnce(makeListIntPtr(), true);
  testListIntPtrPushFrontOnce(makeListIntPtr(), false);
  testListIntPtrPushFrontManyTimes(makeListIntPtr(), true);
  testListIntPtrPushFrontManyTimes(makeListIntPtr(), false);
  testListIntPtrPushFrontOncePopFrontOnce(makeListIntPtr());
  testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(makeListIntPtr());

  testDLListFinalCounts();
  puts("==================");
  puts("DLList tests done.");
  puts("==================");
}
