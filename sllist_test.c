#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "tloc_test.h"

static void testSLListIntConstructDestructStackSpace(void) {
  TloSLList ints;

  TloError error = tloSLListConstruct(&ints, &tloInt, &tloCountingAllocator);
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(&ints) == 0);
  TLO_ASSERT(tloSLListIsEmpty(&ints));
  TLO_ASSERT(tloSLListGetValueType(&ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(&ints) == &tloCountingAllocator);

  tloSLListDestruct(&ints);
}

static void testSLListIntConstructDestructHeapSpace(void) {
  TloSLList *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error = tloSLListConstruct(ints, &tloInt, &tloCountingAllocator);
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(ints) == 0);
  TLO_ASSERT(tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TLO_ASSERT(tloSLListGetSize(ints) == 0);
  TLO_ASSERT(tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListDestructWithNull(void) { tloSLListDestruct(NULL); }

static void testSLListDeleteWithNull(void) { tloSLListDelete(NULL); }

static void testSLListIntPushFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TloError error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(ints) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveFront(ints, &value);
  TLO_ASSERT(!error);

  TLO_ASSERT(value == 0);
  TLO_ASSERT(tloSLListGetSize(ints) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushFront(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    TLO_ASSERT(tloSLListGetSize(ints) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(ints));
    TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
    TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == (int)i);
    TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == 0);
    TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloSLListMoveFront(ints, &value);
    TLO_ASSERT(!error);

    TLO_ASSERT(value == 0);
    TLO_ASSERT(tloSLListGetSize(ints) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(ints));
    TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
    TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == (int)i);
    TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == 0);
    TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontOncePopFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TloError error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  tloSLListPopFront(ints);

  TLO_ASSERT(tloSLListGetSize(ints) == 0);
  TLO_ASSERT(tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushFront(ints, &(int){(int)i});
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_ASSERT(tloSLListGetSize(ints) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(ints));
    TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
    TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == (int)i);
    TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == (int)i);
    TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == 0);
    TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == 0);

    tloSLListPopFront(ints);
  }

  TLO_ASSERT(tloSLListGetSize(ints) == 0);
  TLO_ASSERT(tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TloError error = tloSLListPushBack(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(ints) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveBack(ints, &value);
  TLO_ASSERT(!error);

  TLO_ASSERT(value == 0);
  TLO_ASSERT(tloSLListGetSize(ints) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(ints));
  TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
  TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

  TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == SOME_NUMBER);
  TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    TLO_ASSERT(tloSLListGetSize(ints) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(ints));
    TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
    TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == 0);
    TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == 0);
    TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == (int)i);
    TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tloSLListMoveBack(ints, &value);
    TLO_ASSERT(!error);

    TLO_ASSERT(value == 0);
    TLO_ASSERT(tloSLListGetSize(ints) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(ints));
    TLO_ASSERT(tloSLListGetValueType(ints) == &tloInt);
    TLO_ASSERT(tloSLListGetAllocatorType(ints) == &tloCountingAllocator);

    TLO_ASSERT(*(const int *)tloSLListGetFront(ints) == 0);
    TLO_ASSERT(*(int *)tloSLListGetMutableFront(ints) == 0);
    TLO_ASSERT(*(const int *)tloSLListGetBack(ints) == (int)i);
    TLO_ASSERT(*(int *)tloSLListGetMutableBack(ints) == (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntConstructCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloSLListConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  TLO_ASSERT(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  TLO_ASSERT(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  TLO_ASSERT(tloSLListGetAllocatorType(ints) ==
             tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_ASSERT(elem1 != elem2);
    TLO_ASSERT(*elem1 == *elem2);
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
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMakeCopy(ints);
  TLO_ASSERT(copy);

  TLO_ASSERT(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  TLO_ASSERT(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  TLO_ASSERT(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  TLO_ASSERT(tloSLListGetAllocatorType(ints) ==
             tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_ASSERT(elem1 != elem2);
    TLO_ASSERT(*elem1 == *elem2);
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
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(copy);

  TloError error = tloSLListCopy(copy, ints);
  TLO_ASSERT(!error);

  TLO_ASSERT(tloSLListGetSize(ints) == tloSLListGetSize(copy));
  TLO_ASSERT(tloSLListIsEmpty(ints) == tloSLListIsEmpty(copy));
  TLO_ASSERT(tloSLListGetValueType(ints) == tloSLListGetValueType(copy));
  TLO_ASSERT(tloSLListGetAllocatorType(ints) ==
             tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_ASSERT(elem1 != elem2);
    TLO_ASSERT(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

static void testSLListIntPtrPushFrontOnce(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloSLListPushFront(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  TLO_ASSERT(tloSLListGetSize(intPtrs) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(intPtrs));
  TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
  TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

  TLO_ASSERT(*((const TloIntPtr *)tloSLListGetFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((const TloIntPtr *)tloSLListGetBack(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableBack(intPtrs))->ptr ==
             SOME_NUMBER);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushFrontManyTimes(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloSLListPushFront(intPtrs, &intPtr);
    TLO_ASSERT(!error);

    TLO_ASSERT(tloSLListGetSize(intPtrs) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(intPtrs));
    TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
    TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetFront(intPtrs))->ptr == (int)i);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableFront(intPtrs))->ptr ==
               (int)i);
    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetBack(intPtrs))->ptr == 0);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableBack(intPtrs))->ptr == 0);
  }
  tloPtrDestruct(&intPtr);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushFrontOncePopFrontOnce(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloSLListPushFront(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tloSLListPopFront(intPtrs);

  TLO_ASSERT(tloSLListGetSize(intPtrs) == 0);
  TLO_ASSERT(tloSLListIsEmpty(intPtrs));
  TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
  TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloSLListPushFront(intPtrs, &intPtr);
    TLO_ASSERT(!error);
  }
  tloPtrDestruct(&intPtr);

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    TLO_ASSERT(tloSLListGetSize(intPtrs) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(intPtrs));
    TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
    TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetFront(intPtrs))->ptr == (int)i);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableFront(intPtrs))->ptr ==
               (int)i);
    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetBack(intPtrs))->ptr == 0);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableBack(intPtrs))->ptr == 0);

    tloSLListPopFront(intPtrs);
  }

  TLO_ASSERT(tloSLListGetSize(intPtrs) == 0);
  TLO_ASSERT(tloSLListIsEmpty(intPtrs));
  TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
  TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushBackOnce(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  *intPtr.ptr = SOME_NUMBER;
  error = tloSLListPushBack(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  TLO_ASSERT(tloSLListGetSize(intPtrs) == 1);
  TLO_ASSERT(!tloSLListIsEmpty(intPtrs));
  TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
  TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

  TLO_ASSERT(*((const TloIntPtr *)tloSLListGetFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableFront(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((const TloIntPtr *)tloSLListGetBack(intPtrs))->ptr ==
             SOME_NUMBER);
  TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableBack(intPtrs))->ptr ==
             SOME_NUMBER);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushBackManyTimes(void) {
  TloSLList *intPtrs = tloSLListMake(&tloIntPtr, &tloCountingAllocator);
  TLO_ASSERT(intPtrs);

  TloIntPtr intPtr;
  TloError error = tloIntPtrConstruct(&intPtr);
  TLO_ASSERT(!error);
  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    *intPtr.ptr = (int)i;
    error = tloSLListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);

    TLO_ASSERT(tloSLListGetSize(intPtrs) == i + 1);
    TLO_ASSERT(!tloSLListIsEmpty(intPtrs));
    TLO_ASSERT(tloSLListGetValueType(intPtrs) == &tloIntPtr);
    TLO_ASSERT(tloSLListGetAllocatorType(intPtrs) == &tloCountingAllocator);

    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetFront(intPtrs))->ptr == 0);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableFront(intPtrs))->ptr == 0);
    TLO_ASSERT(*((const TloIntPtr *)tloSLListGetBack(intPtrs))->ptr == (int)i);
    TLO_ASSERT(*((TloIntPtr *)tloSLListGetMutableBack(intPtrs))->ptr == (int)i);
  }
  tloPtrDestruct(&intPtr);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

void testSLList(void) {
  tloCountingAllocatorResetCounts();
  TLO_ASSERT(tloCountingAllocatorGetMallocCount() == 0);
  TLO_ASSERT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_ASSERT(tloCountingAllocatorGetTotalByteCount() == 0);

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
  testSLListIntPtrPushFrontOnce();
  testSLListIntPtrPushFrontManyTimes();
  testSLListIntPtrPushFrontOncePopFrontOnce();
  testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty();
  testSLListIntPtrPushBackOnce();
  testSLListIntPtrPushBackManyTimes();

  TLO_ASSERT(tloCountingAllocatorGetMallocCount() > 0);
  TLO_ASSERT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_ASSERT(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloSLList): %zu\n", sizeof(TloSLList));
  printf("sizeof(TloSLLNode): %zu\n", sizeof(TloSLLNode));
  tloCountingAllocatorPrintCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
