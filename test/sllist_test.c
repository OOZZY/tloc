#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "tloc_test.h"
#include "util.h"

static void testSLListInitialCounts() {
  countingAllocatorResetCounts();
  TLO_EXPECT(countingAllocatorMallocCount() == 0);
  TLO_EXPECT(countingAllocatorMallocCount() == countingAllocatorFreeCount());
  TLO_EXPECT(countingAllocatorTotalByteCount() == 0);
}

#define EXPECT_SLLIST_PROPERTIES(sllist, size, isEmpty, valueType, \
                                 allocatorType)                    \
  do {                                                             \
    TLO_EXPECT(tloSLListSize(sllist) == (size));                   \
    TLO_EXPECT(tloSLListIsEmpty(sllist) == (isEmpty));             \
    TLO_EXPECT(tloSLListValueType(sllist) == (valueType));         \
    TLO_EXPECT(tloSLListAllocatorType(sllist) == (allocatorType)); \
  } while (0)

static void testSLListIntConstructDestruct(void) {
  TloSLList ints;

  TloError error = tloSLListConstruct(&ints, &tloInt, &countingAllocator);
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(&ints, 0, true, &tloInt, &countingAllocator);

  tloSLListDestruct(&ints);
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListDestructWithNull(void) { tloSLListDestruct(NULL); }

static void testSLListDeleteWithNull(void) { tloSLListDelete(NULL); }

#define EXPECT_SLLIST_INT_ELEMENTS(sllist, frontValue, backValue)      \
  do {                                                                 \
    TLO_EXPECT(*(const int *)tloSLListFront(sllist) == (frontValue));  \
    TLO_EXPECT(*(int *)tloSLListMutableFront(sllist) == (frontValue)); \
    TLO_EXPECT(*(const int *)tloSLListBack(sllist) == (backValue));    \
    TLO_EXPECT(*(int *)tloSLListMutableBack(sllist) == (backValue));   \
  } while (0)

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

  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
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

    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, (int)i, 0);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontOncePopFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListPushFront(ints, &value);
  TLO_ASSERT(!error);

  tloSLListPopFront(ints);

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloSLListDelete(ints);
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
    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, (int)i, 0);

    tloSLListPopFront(ints);
  }

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushOrMoveBackOnce(bool testPush) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  TloError error;
  if (testPush) {
    int value = SOME_NUMBER;
    error = tloSLListPushBack(ints, &value);
  } else {
    int *value = makeInt(SOME_NUMBER);
    TLO_ASSERT(value);
    error = tloSLListMoveBack(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushOrMoveBackManyTimes(bool testPush) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      int value = (int)i;
      error = tloSLListPushBack(ints, &value);
    } else {
      int *value = makeInt((int)i);
      TLO_ASSERT(value);
      error = tloSLListMoveBack(ints, value);
    }
    TLO_ASSERT(!error);

    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, 0, (int)i);
  }

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntConstructCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = malloc(sizeof(*copy));
  TLO_ASSERT(copy);

  TloError error = tloSLListConstructCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListSize(copy), tloSLListIsEmpty(copy),
                           tloSLListValueType(copy),
                           tloSLListAllocatorType(copy));

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

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDestruct(copy);
  free(copy);
  copy = NULL;
}

static void testSLListIntMakeCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMakeCopy(ints);
  TLO_ASSERT(copy);

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListSize(copy), tloSLListIsEmpty(copy),
                           tloSLListValueType(copy),
                           tloSLListAllocatorType(copy));

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

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

static void testSLListIntCopy(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &i);
    TLO_ASSERT(!error);
  }

  TloSLList *copy = tloSLListMake(&tloInt, &countingAllocator);
  TLO_ASSERT(copy);

  TloError error = tloSLListCopy(copy, ints);
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListSize(copy), tloSLListIsEmpty(copy),
                           tloSLListValueType(copy),
                           tloSLListAllocatorType(copy));

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

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

#define EXPECT_SLLIST_INTPTR_ELEMENTS(sllist, frontValue, backValue)           \
  do {                                                                         \
    TLO_EXPECT(*((const IntPtr *)tloSLListFront(sllist))->ptr ==               \
               (frontValue));                                                  \
    TLO_EXPECT(*((IntPtr *)tloSLListMutableFront(sllist))->ptr ==              \
               (frontValue));                                                  \
    TLO_EXPECT(*((const IntPtr *)tloSLListBack(sllist))->ptr == (backValue));  \
    TLO_EXPECT(*((IntPtr *)tloSLListMutableBack(sllist))->ptr == (backValue)); \
  } while (0)

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

  EXPECT_SLLIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_SLLIST_INTPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(intPtrs);
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

    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_SLLIST_INTPTR_ELEMENTS(intPtrs, (int)i, 0);
  }

  tloSLListDelete(intPtrs);
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

  EXPECT_SLLIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloSLListDelete(intPtrs);
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
    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_SLLIST_INTPTR_ELEMENTS(intPtrs, (int)i, 0);

    tloSLListPopFront(intPtrs);
  }

  EXPECT_SLLIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushOrMoveBackOnce(bool testPush) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  TloError error;
  if (testPush) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, SOME_NUMBER);
    TLO_ASSERT(!error);
    error = tloSLListPushBack(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(SOME_NUMBER);
    TLO_ASSERT(intPtr);
    error = tloSLListMoveBack(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_SLLIST_INTPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

static void testSLListIntPtrPushOrMoveBackManyTimes(bool testPush) {
  TloSLList *intPtrs = tloSLListMake(&intPtrType, &countingAllocator);
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      IntPtr intPtr;
      error = intPtrConstruct(&intPtr, (int)i);
      TLO_ASSERT(!error);
      error = tloSLListPushBack(intPtrs, &intPtr);
      tloPtrDestruct(&intPtr);
    } else {
      IntPtr *intPtr = intPtrMake((int)i);
      TLO_ASSERT(intPtr);
      error = tloSLListMoveBack(intPtrs, intPtr);
    }
    TLO_ASSERT(!error);

    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                             &countingAllocator);
    EXPECT_SLLIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i);
  }

  tloSLListDelete(intPtrs);
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
  testSLListDestructWithNull();
  testSLListDeleteWithNull();
  testSLListIntPushOrMoveFrontOnce(true);
  testSLListIntPushOrMoveFrontOnce(false);
  testSLListIntPushOrMoveFrontManyTimes(true);
  testSLListIntPushOrMoveFrontManyTimes(false);
  testSLListIntPushFrontOncePopFrontOnce();
  testSLListIntPushFrontManyTimesPopFrontUntilEmpty();
  testSLListIntPushOrMoveBackOnce(true);
  testSLListIntPushOrMoveBackOnce(false);
  testSLListIntPushOrMoveBackManyTimes(true);
  testSLListIntPushOrMoveBackManyTimes(false);
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();
  testSLListIntPtrPushOrMoveFrontOnce(true);
  testSLListIntPtrPushOrMoveFrontOnce(false);
  testSLListIntPtrPushOrMoveFrontManyTimes(true);
  testSLListIntPtrPushOrMoveFrontManyTimes(false);
  testSLListIntPtrPushFrontOncePopFrontOnce();
  testSLListIntPtrPushFrontManyTimesPopFrontUntilEmpty();
  testSLListIntPtrPushOrMoveBackOnce(true);
  testSLListIntPtrPushOrMoveBackOnce(false);
  testSLListIntPtrPushOrMoveBackManyTimes(true);
  testSLListIntPtrPushOrMoveBackManyTimes(false);
  testSLListFinalCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
