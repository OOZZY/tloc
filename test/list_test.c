#include "list_test.h"

void testListIntPushOrMoveBackOnce(TloList *ints, bool testPush) {
  TLO_ASSERT(ints);

  TloError error;
  if (testPush) {
    int value = SOME_NUMBER;
    error = tlovListPushBack(ints, &value);
  } else {
    int *value = makeInt(SOME_NUMBER);
    TLO_ASSERT(value);
    error = tlovListMoveBack(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER, 0, SOME_NUMBER);

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushOrMoveBackManyTimes(TloList *ints, bool testPush) {
  TLO_ASSERT(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      int value = (int)i;
      error = tlovListPushBack(ints, &value);
    } else {
      int *value = makeInt((int)i);
      TLO_ASSERT(value);
      error = tlovListMoveBack(ints, value);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, 0, (int)i, i, (int)i);
  }

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPtrPushOrMoveBackOnce(TloList *intPtrs, bool testPush) {
  TLO_ASSERT(intPtrs);

  TloError error;
  if (testPush) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, SOME_NUMBER);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(SOME_NUMBER);
    TLO_ASSERT(intPtr);
    error = tlovListMoveBack(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER, 0,
                              SOME_NUMBER);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushOrMoveBackManyTimes(TloList *intPtrs, bool testPush) {
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      IntPtr intPtr;
      error = intPtrConstruct(&intPtr, (int)i);
      TLO_ASSERT(!error);
      error = tlovListPushBack(intPtrs, &intPtr);
      tloPtrDestruct(&intPtr);
    } else {
      IntPtr *intPtr = intPtrMake((int)i);
      TLO_ASSERT(intPtr);
      error = tlovListMoveBack(intPtrs, intPtr);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i, i, (int)i);
  }

  tloListDelete(intPtrs);
  intPtrs = NULL;
}
