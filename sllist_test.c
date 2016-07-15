#include "libtloc_test.h"
#include <tlo/sllist.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define SOME_NUMBER 42

static void testSLListIntConstructDestructStackSpace(void) {
  TloSLList ints;

  tloError error =
      tloSLListConstruct(&ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloSLListGetSize(&ints) == 0);
  assert(tloSLListIsEmpty(&ints));
  assert(tloSLListGetType(&ints) == &tloIntType);
  assert(tloSLListGetAllocator(&ints) == &tloCountingAllocator);

  tloSLListDestruct(&ints);
}

static void testSLListIntConstructDestructHeapSpace(void) {
  TloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  tloError error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListDestructWithNull(void) { tloSLListDestruct(NULL); }

static void testSLListDeleteWithNull(void) { tloSLListDelete(NULL); }

static void testSLListIntPushFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  tloError error = tloSLListPushFront(ints, &(int) {SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  tloError error = tloSLListMoveFront(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushFront(ints, &(int) {(int)i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    tloError error = tloSLListMoveFront(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontOncePopFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  tloError error = tloSLListPushFront(ints, &(int) {SOME_NUMBER});
  assert(!error);

  tloSLListPopFront(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushFront(ints, &(int) {(int)i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetMutableBack(ints) == 0);

    tloSLListPopFront(ints);
  }

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  tloError error = tloSLListPushBack(ints, &(int) {SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  tloError error = tloSLListMoveBack(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushBack(ints, &(int) {(int)i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetMutableFront(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    tloError error = tloSLListMoveBack(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetMutableFront(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int)i);
    assert(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntConstructCopy(void) {
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = malloc(sizeof(*copy));
  assert(copy);

  tloError error = tloSLListConstructCopy(copy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetType(ints) == tloSLListGetType(copy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(copy));

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
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = tloSLListMakeCopy(ints);
  assert(copy);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetType(ints) == tloSLListGetType(copy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(copy));

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
  TloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    tloError error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  TloSLList *copy = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(copy);

  tloError error = tloSLListCopy(copy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  assert(tloSLListGetType(ints) == tloSLListGetType(copy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(copy));

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
