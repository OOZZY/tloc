#include <stdio.h>
#include <stdlib.h>
#include <tlo/sllist.h>
#include <tlo/test.h>
#include "tloc_test.h"

#define EXPECT_SLLIST_PROPERTIES(sllist, size, isEmpty, valueType,    \
                                 allocatorType)                       \
  do {                                                                \
    TLO_EXPECT(tloSLListGetSize(sllist) == (size));                   \
    TLO_EXPECT(tloSLListIsEmpty(sllist) == (isEmpty));                \
    TLO_EXPECT(tloSLListGetValueType(sllist) == (valueType));         \
    TLO_EXPECT(tloSLListGetAllocatorType(sllist) == (allocatorType)); \
  } while (0)

static void testSLListIntConstructDestructStackSpace(void) {
  TloSLList ints;

  TloError error = tloSLListConstruct(&ints, &tloInt, &tloCountingAllocator);
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(&ints, 0, true, &tloInt, &tloCountingAllocator);

  tloSLListDestruct(&ints);
}

static void testSLListIntConstructDestructHeapSpace(void) {
  TloSLList *ints = malloc(sizeof(*ints));
  TLO_ASSERT(ints);

  TloError error = tloSLListConstruct(ints, &tloInt, &tloCountingAllocator);
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);
  ints = NULL;
}

static void testSLListIntMakeDelete(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListDestructWithNull(void) { tloSLListDestruct(NULL); }

static void testSLListDeleteWithNull(void) { tloSLListDelete(NULL); }

#define EXPECT_SLLIST_INT_ELEMENTS(sllist, frontValue, backValue)         \
  do {                                                                    \
    TLO_EXPECT(*(const int *)tloSLListGetFront(sllist) == (frontValue));  \
    TLO_EXPECT(*(int *)tloSLListGetMutableFront(sllist) == (frontValue)); \
    TLO_EXPECT(*(const int *)tloSLListGetBack(sllist) == (backValue));    \
    TLO_EXPECT(*(int *)tloSLListGetMutableBack(sllist) == (backValue));   \
  } while (0)

static void testSLListIntPushFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TloError error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveFrontOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveFront(ints, &value);
  TLO_ASSERT(!error);

  TLO_EXPECT(value == 0);
  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushFrontManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushFront(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, (int)i, 0);
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

    TLO_EXPECT(value == 0);
    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, (int)i, 0);
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

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

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
    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, (int)i, 0);

    tloSLListPopFront(ints);
  }

  EXPECT_SLLIST_PROPERTIES(ints, 0, true, &tloInt, &tloCountingAllocator);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  TloError error = tloSLListPushBack(ints, &(int){SOME_NUMBER});
  TLO_ASSERT(!error);

  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntMoveBackOnce(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  int value = SOME_NUMBER;
  TloError error = tloSLListMoveBack(ints, &value);
  TLO_ASSERT(!error);

  TLO_EXPECT(value == 0);
  EXPECT_SLLIST_PROPERTIES(ints, 1, false, &tloInt, &tloCountingAllocator);
  EXPECT_SLLIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER);

  tloSLListDelete(ints);
  ints = NULL;
}

static void testSLListIntPushBackManyTimes(void) {
  TloSLList *ints = tloSLListMake(&tloInt, &tloCountingAllocator);
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error = tloSLListPushBack(ints, &(int){(int)i});
    TLO_ASSERT(!error);

    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, 0, (int)i);
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

    TLO_EXPECT(value == 0);
    EXPECT_SLLIST_PROPERTIES(ints, i + 1, false, &tloInt,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INT_ELEMENTS(ints, 0, (int)i);
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

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListGetSize(copy), tloSLListIsEmpty(copy),
                           tloSLListGetValueType(copy),
                           tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
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

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListGetSize(copy), tloSLListIsEmpty(copy),
                           tloSLListGetValueType(copy),
                           tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
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

  EXPECT_SLLIST_PROPERTIES(ints, tloSLListGetSize(copy), tloSLListIsEmpty(copy),
                           tloSLListGetValueType(copy),
                           tloSLListGetAllocatorType(copy));

  const TloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const TloSLLNode *node2 = tloSLLNodeGetHead(copy);
  while (node1) {
    TLO_ASSERT(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    TLO_EXPECT(elem1 != elem2);
    TLO_EXPECT(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  ints = NULL;

  tloSLListDelete(copy);
  copy = NULL;
}

#define EXPECT_SLLIST_INPTR_ELEMENTS(sllist, frontValue, backValue)     \
  do {                                                                  \
    TLO_EXPECT(*((const TloIntPtr *)tloSLListGetFront(sllist))->ptr ==  \
               (frontValue));                                           \
    TLO_EXPECT(*((TloIntPtr *)tloSLListGetMutableFront(sllist))->ptr == \
               (frontValue));                                           \
    TLO_EXPECT(*((const TloIntPtr *)tloSLListGetBack(sllist))->ptr ==   \
               (backValue));                                            \
    TLO_EXPECT(*((TloIntPtr *)tloSLListGetMutableBack(sllist))->ptr ==  \
               (backValue));                                            \
  } while (0)

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

  EXPECT_SLLIST_PROPERTIES(intPtrs, 1, false, &tloIntPtr,
                           &tloCountingAllocator);
  EXPECT_SLLIST_INPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER);

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

    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INPTR_ELEMENTS(intPtrs, (int)i, 0);
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

  EXPECT_SLLIST_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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
    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INPTR_ELEMENTS(intPtrs, (int)i, 0);

    tloSLListPopFront(intPtrs);
  }

  EXPECT_SLLIST_PROPERTIES(intPtrs, 0, true, &tloIntPtr, &tloCountingAllocator);

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

  EXPECT_SLLIST_PROPERTIES(intPtrs, 1, false, &tloIntPtr,
                           &tloCountingAllocator);
  EXPECT_SLLIST_INPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER);

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

    EXPECT_SLLIST_PROPERTIES(intPtrs, i + 1, false, &tloIntPtr,
                             &tloCountingAllocator);
    EXPECT_SLLIST_INPTR_ELEMENTS(intPtrs, 0, (int)i);
  }
  tloPtrDestruct(&intPtr);

  tloSLListDelete(intPtrs);
  intPtrs = NULL;
}

void testSLList(void) {
  tloCountingAllocatorResetCounts();
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() == 0);
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_EXPECT(tloCountingAllocatorGetTotalByteCount() == 0);

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

  TLO_EXPECT(tloCountingAllocatorGetMallocCount() > 0);
  TLO_EXPECT(tloCountingAllocatorGetMallocCount() ==
             tloCountingAllocatorGetFreeCount());
  TLO_EXPECT(tloCountingAllocatorGetTotalByteCount() > 0);

  printf("sizeof(TloSLList): %zu\n", sizeof(TloSLList));
  printf("sizeof(TloSLLNode): %zu\n", sizeof(TloSLLNode));
  tloCountingAllocatorPrintCounts();
  puts("==================");
  puts("SLList tests done.");
  puts("==================");
}
