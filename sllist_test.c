#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include "libtloc_test.h"

#define SOME_NUMBER 42

static void testSLListIntConstructDestructStackSpace(void) {
  TloSLList ints;

  TloError error = tloSLListConstruct(&ints, &tloInt, &tloCountingAllocator);
  assert(!error);

  assert(tloSLListGetSize(&ints) == 0);
  assert(tloSLListIsEmpty(&ints));
  assert(tloSLListGetValueType(&ints) == &tloInt);
  assert(tloSLListGetAllocatorType(&ints) == &tloCountingAllocator);

  tloSLListDestruct(&ints);
}

static void testSLListIntConstructDestructHeapSpace(void) {
  TloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  TloError error = tloSLListConstruct(ints, &tloInt, &tloCountingAllocator);
  assert(!error);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListDestructWithNull(void) { tloSLListDestruct(NULL); }

static void testSLListDeleteWithNull(void) { tloSLListDelete(NULL); }

static void testSLListIntPushFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  TloError error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveFront(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushFront(ints, &(int){(int)i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetValueType(ints) == &tloInt);
    assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloSLListMoveFront(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetValueType(ints) == &tloInt);
    assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontOncePopFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  TloError error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  assert(!error);

  tloSLListPopFront(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushFront(ints, &(int){(int)i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetValueType(ints) == &tloInt);
    assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);

    tloSLListPopFront(ints);
  }

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  TloError error = tloSLListPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveBack(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetValueType(ints) == &tloInt);
  assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &(int){(int)i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetValueType(ints) == &tloInt);
    assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetMutableFront(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloSLListMoveBack(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetValueType(ints) == &tloInt);
    assert(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetMutableFront(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntConstructCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = malloc(sizeof(*copy));
  assert(copy);

  TloError error = tloSLListConstructCopy(copy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  assert(tloSLListGetAllocatorType(ints) == tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDestruct(copy);
  free(copy);
  copy = NULL;
}

static void testSLListIntMakeCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = tloSLListMakeCopy(ints);
  assert(copy);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  assert(tloSLListGetAllocatorType(ints) == tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

static void testSLListIntCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = tloSLListMake(&tloInt, &tloCountingAllocator);
  assert(copy);

  TloError error = tloSLListCopy(copy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  assert(tloSLListGetAllocatorType(ints) == tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

void testSLList(void) {
  tloCountingAllocatorResetCounts();
  assert(tloCountingAllocatorGetMallocCount() == 0);
  assert(tloCountingAllocatorGetMallocCount() ==
         tloCountingAllocatorGetFreeCount());
  assert(tloCountingAllocatorGetTotalByteCount() == 0);

  testSLListIntConstructDestructStackSpace();
  testSLListIntConstructDestructHeapSpace();
  testSLListIntMakeDelete();
  testSLListDestructWithNull();
  testSLListDeleteWithNull();
  testSLListIntPushFrontOnce();
  testSLListIntMoveFrontOnce();
  testSLListIntPushFrontManyTimes();
  testSLListIntMoveFrontManyTimes();
  testSLListIntPushFrontOncePopFrontOnce();
  testSLListIntPushFrontManyTimesPopFrontUntilEmpty();
  testSLListIntPushBackOnce();
  testSLListIntMoveBackOnce();
  testSLListIntPushBackManyTimes();
  testSLListIntMoveBackManyTimes();
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();
  // testSLListIntPtrPushFrontOnce();
  // testSLListIntPtrPushFrontManyTimes();
  // testSLListIntPtrPushFrontOncePopFrontOnce();
  // testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty();
  // testSLListIntPtrPushBackOnce();
  // testSLListIntPtrPushBackManyTimes();

  assert(tloCountingAllocatorGetMallocCount() > 0);
  assert(tloCountingAllocatorGetMallocCount() ==
         tloCountingAllocatorGetFreeCount());
  assert(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloSLList): %zu\n", sizeof(TloSLList));
  printf("sizeof(TloSLLNode): %zu\n", sizeof(TloSLLNode));
  printf("malloc count: %lu\nfree count: %lu\ntotal bytes allocated: %lu\n",
         tloCountingAllocatorGetMallocCount(),
         tloCountingAllocatorGetFreeCount(),
         tloCountingAllocatorGetTotalByteCount());
  puts("SLList tests passed.");
}
