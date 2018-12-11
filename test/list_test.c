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

void testListIntPushBackOncePopBackOnce(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_POP_BACK));

  int value = SOME_NUMBER;
  TloError error = tlovListPushBack(ints, &value);
  TLO_ASSERT(!error);

  tlovListPopBack(ints);

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushBackManyTimesPopBackUntilEmpty(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tlovListPushBack(ints, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, 0, (int)i, i, (int)i);

    tlovListPopBack(ints);
  }

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushOrMoveFrontOnce(TloList *ints, bool testPush) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT));

  TloError error;
  if (testPush) {
    int value = SOME_NUMBER;
    error = tlovListPushFront(ints, &value);
  } else {
    int *value = makeInt(SOME_NUMBER);
    TLO_ASSERT(value);
    error = tlovListMoveFront(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(ints, SOME_NUMBER, SOME_NUMBER, 0, SOME_NUMBER);

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushOrMoveFrontManyTimes(TloList *ints, bool testPush) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      int value = (int)i;
      error = tlovListPushFront(ints, &value);
    } else {
      int *value = makeInt((int)i);
      TLO_ASSERT(value);
      error = tlovListMoveFront(ints, value);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, (int)i, 0, i, 0);
  }

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushFrontOncePopFrontOnce(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(
      tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  int value = SOME_NUMBER;
  TloError error = tlovListPushFront(ints, &value);
  TLO_ASSERT(!error);

  tlovListPopFront(ints);

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
  ints = NULL;
}

void testListIntPushFrontManyTimesPopFrontUntilEmpty(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(
      tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    TloError error = tlovListPushFront(ints, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, (int)i, 0, i, 0);

    tlovListPopFront(ints);
  }

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

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

void testListIntPtrPushBackOncePopBackOnce(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_POP_BACK));

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, SOME_NUMBER);
  TLO_ASSERT(!error);
  error = tlovListPushBack(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tlovListPopBack(intPtrs);

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushBackManyTimesPopBackUntilEmpty(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i, i, (int)i);

    tlovListPopBack(intPtrs);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
    TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i, i, (int)i);

    tlovListUnorderedRemove(intPtrs, tlovListSize(intPtrs) - 1);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
    TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);

    if (i == SOME_NUMBER - 1) {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, SOME_NUMBER - 1, i,
                                  SOME_NUMBER - 1);
    } else if (i == 0) {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 1, 1, i, 1);
    } else {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, (int)i + 1, (int)i, i, (int)i);
    }

    tlovListUnorderedRemove(intPtrs, 0);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushOrMoveFrontOnce(TloList *intPtrs, bool testPush) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT));

  TloError error;
  if (testPush) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, SOME_NUMBER);
    TLO_ASSERT(!error);
    error = tlovListPushFront(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(SOME_NUMBER);
    TLO_ASSERT(intPtr);
    error = tlovListMoveFront(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, SOME_NUMBER, SOME_NUMBER, 0,
                              SOME_NUMBER);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushOrMoveFrontManyTimes(TloList *intPtrs, bool testPush) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    TloError error;
    if (testPush) {
      IntPtr intPtr;
      error = intPtrConstruct(&intPtr, (int)i);
      TLO_ASSERT(!error);
      error = tlovListPushFront(intPtrs, &intPtr);
      tloPtrDestruct(&intPtr);
    } else {
      IntPtr *intPtr = intPtrMake((int)i);
      TLO_ASSERT(intPtr);
      error = tlovListMoveFront(intPtrs, intPtr);
    }
    TLO_ASSERT(!error);

    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, (int)i, 0, i, 0);
  }

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushFrontOncePopFrontOnce(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(
      tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, SOME_NUMBER);
  TLO_ASSERT(!error);
  error = tlovListPushFront(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tlovListPopFront(intPtrs);

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}

void testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(
      tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushFront(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, (int)i, 0, i, 0);

    tlovListPopFront(intPtrs);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
  intPtrs = NULL;
}
