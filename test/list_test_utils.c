#include "list_test_utils.h"

void testListDeleteWithNull(void) { tloListDelete(NULL); }

void testListHasFunctions(TloList *list, unsigned char functions) {
  TLO_ASSERT(list);
  TLO_EXPECT(tloListHasFunctions(list, functions));
  tloListDelete(list);
}

void testListIntPushOrMoveBackOnce(TloList *ints, bool testCopy) {
  TLO_ASSERT(ints);

  TloError error;
  if (testCopy) {
    int value = MAX_LIST_SIZE;
    error = tlovListPushBack(ints, &value);
  } else {
    int *value = makeInt(MAX_LIST_SIZE);
    TLO_ASSERT(value);
    error = tlovListMoveBack(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(ints, MAX_LIST_SIZE, MAX_LIST_SIZE, 0,
                           MAX_LIST_SIZE);

  tloListDelete(ints);
}

void testListIntPushOrMoveBackManyTimes(TloList *ints, bool testCopy) {
  TLO_ASSERT(ints);

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error;
    if (testCopy) {
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
}

void testListIntPushBackOncePopBackOnce(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_POP_BACK));

  int value = MAX_LIST_SIZE;
  TloError error = tlovListPushBack(ints, &value);
  TLO_ASSERT(!error);

  tlovListPopBack(ints);

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
}

void testListIntPushBackManyTimesPopBackUntilEmpty(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    int value = (int)i;
    TloError error = tlovListPushBack(ints, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, 0, (int)i, i, (int)i);

    tlovListPopBack(ints);
  }

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
}

void testListIntPushOrMoveFrontOnce(TloList *ints, bool testCopy) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT));

  TloError error;
  if (testCopy) {
    int value = MAX_LIST_SIZE;
    error = tlovListPushFront(ints, &value);
  } else {
    int *value = makeInt(MAX_LIST_SIZE);
    TLO_ASSERT(value);
    error = tlovListMoveFront(ints, value);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);
  EXPECT_LIST_INT_ELEMENTS(ints, MAX_LIST_SIZE, MAX_LIST_SIZE, 0,
                           MAX_LIST_SIZE);

  tloListDelete(ints);
}

void testListIntPushOrMoveFrontManyTimes(TloList *ints, bool testCopy) {
  TLO_ASSERT(ints);
  TLO_ASSERT(tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error;
    if (testCopy) {
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
}

void testListIntPushFrontOncePopFrontOnce(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(
      tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  int value = MAX_LIST_SIZE;
  TloError error = tlovListPushFront(ints, &value);
  TLO_ASSERT(!error);

  tlovListPopFront(ints);

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
}

void testListIntPushFrontManyTimesPopFrontUntilEmpty(TloList *ints) {
  TLO_ASSERT(ints);
  TLO_ASSERT(
      tloListHasFunctions(ints, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    int value = (int)i;
    TloError error = tlovListPushFront(ints, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);
    EXPECT_LIST_INT_ELEMENTS(ints, (int)i, 0, i, 0);

    tlovListPopFront(ints);
  }

  EXPECT_LIST_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloListDelete(ints);
}

void testListIntPtrPushOrMoveBackOnce(TloList *intPtrs, bool testCopy) {
  TLO_ASSERT(intPtrs);

  TloError error;
  if (testCopy) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, MAX_LIST_SIZE);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(MAX_LIST_SIZE);
    TLO_ASSERT(intPtr);
    error = tlovListMoveBack(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, MAX_LIST_SIZE, MAX_LIST_SIZE, 0,
                              MAX_LIST_SIZE);

  tloListDelete(intPtrs);
}

void testListIntPtrPushOrMoveBackManyTimes(TloList *intPtrs, bool testCopy) {
  TLO_ASSERT(intPtrs);

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error;
    if (testCopy) {
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
}

void testListIntPtrPushBackOncePopBackOnce(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_POP_BACK));

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, MAX_LIST_SIZE);
  TLO_ASSERT(!error);
  error = tlovListPushBack(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tlovListPopBack(intPtrs);

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}

void testListIntPtrPushBackManyTimesPopBackUntilEmpty(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_POP_BACK));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i, i, (int)i);

    tlovListPopBack(intPtrs);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}

void testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
    TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, (int)i, i, (int)i);

    tlovListUnorderedRemove(intPtrs, tlovListSize(intPtrs) - 1);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}

void testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
    TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_UNORDERED_REMOVE));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushBack(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);

    if (i == MAX_LIST_SIZE - 1) {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 0, MAX_LIST_SIZE - 1, i,
                                  MAX_LIST_SIZE - 1);
    } else if (i == 0) {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, 1, 1, i, 1);
    } else {
      EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, (int)i + 1, (int)i, i, (int)i);
    }

    tlovListUnorderedRemove(intPtrs, 0);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}

void testListIntPtrPushOrMoveFrontOnce(TloList *intPtrs, bool testCopy) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT));

  TloError error;
  if (testCopy) {
    IntPtr intPtr;
    error = intPtrConstruct(&intPtr, MAX_LIST_SIZE);
    TLO_ASSERT(!error);
    error = tlovListPushFront(intPtrs, &intPtr);
    tloPtrDestruct(&intPtr);
  } else {
    IntPtr *intPtr = intPtrMake(MAX_LIST_SIZE);
    TLO_ASSERT(intPtr);
    error = tlovListMoveFront(intPtrs, intPtr);
  }
  TLO_ASSERT(!error);

  EXPECT_LIST_PROPERTIES(intPtrs, 1, false, &intPtrType, &countingAllocator);
  EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, MAX_LIST_SIZE, MAX_LIST_SIZE, 0,
                              MAX_LIST_SIZE);

  tloListDelete(intPtrs);
}

void testListIntPtrPushOrMoveFrontManyTimes(TloList *intPtrs, bool testCopy) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    TloError error;
    if (testCopy) {
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
}

void testListIntPtrPushFrontOncePopFrontOnce(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(
      tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  IntPtr intPtr;
  TloError error = intPtrConstruct(&intPtr, MAX_LIST_SIZE);
  TLO_ASSERT(!error);
  error = tlovListPushFront(intPtrs, &intPtr);
  TLO_ASSERT(!error);
  tloPtrDestruct(&intPtr);

  tlovListPopFront(intPtrs);

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}

void testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(TloList *intPtrs) {
  TLO_ASSERT(intPtrs);
  TLO_ASSERT(
      tloListHasFunctions(intPtrs, TLO_LIST_PUSH_FRONT | TLO_LIST_POP_FRONT));

  for (size_t i = 0; i < MAX_LIST_SIZE; ++i) {
    IntPtr intPtr;
    TloError error = intPtrConstruct(&intPtr, (int)i);
    TLO_ASSERT(!error);
    error = tlovListPushFront(intPtrs, &intPtr);
    TLO_ASSERT(!error);
    tloPtrDestruct(&intPtr);
  }

  for (size_t i = MAX_LIST_SIZE - 1; i <= MAX_LIST_SIZE - 1; --i) {
    EXPECT_LIST_PROPERTIES(intPtrs, i + 1, false, &intPtrType,
                           &countingAllocator);
    EXPECT_LIST_INTPTR_ELEMENTS(intPtrs, (int)i, 0, i, 0);

    tlovListPopFront(intPtrs);
  }

  EXPECT_LIST_PROPERTIES(intPtrs, 0, true, &intPtrType, &countingAllocator);

  tloListDelete(intPtrs);
}
