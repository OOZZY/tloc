#include "set_test_utils.h"

void testSetIntInsertOnce(TloSet *ints, bool testCopy) {
  TLO_ASSERT(ints);

  TloError error;

  if (testCopy) {
    int key = MAX_SET_SIZE;
    error = tlovSetInsert(ints, &key);
    TLO_ASSERT(!error);

    error = tlovSetInsert(ints, &key);
    TLO_ASSERT(error == TLO_DUPLICATE);
  } else {
    int *key = makeInt(MAX_SET_SIZE);
    TLO_ASSERT(key);
    error = tlovSetMoveInsert(ints, key);
    TLO_ASSERT(!error);

    key = makeInt(MAX_SET_SIZE);
    TLO_ASSERT(key);
    error = tlovSetMoveInsert(ints, key);
    TLO_ASSERT(error == TLO_DUPLICATE);
    countingAllocator.free(key);
  }

  EXPECT_SET_PROPERTIES(ints, 1, false, &tloInt, &countingAllocator);

  int key = MAX_SET_SIZE;
  const void *result = tlovSetFind(ints, &key);
  TLO_EXPECT(result && *(const int *)result == key);

  tloSetDelete(ints);
}

void testSetIntInsertManyTimes(TloSet *ints, bool testCopy) {
  TLO_ASSERT(ints);

  for (size_t i = 0; i < MAX_SET_SIZE; ++i) {
    TloError error;

    if (testCopy) {
      int key = (int)i;
      error = tlovSetInsert(ints, &key);
      TLO_ASSERT(!error);

      error = tlovSetInsert(ints, &key);
      TLO_ASSERT(error == TLO_DUPLICATE);
    } else {
      int *key = makeInt((int)i);
      TLO_ASSERT(key);
      error = tlovSetMoveInsert(ints, key);
      TLO_ASSERT(!error);

      key = makeInt((int)i);
      TLO_ASSERT(key);
      error = tlovSetMoveInsert(ints, key);
      TLO_ASSERT(error == TLO_DUPLICATE);
      countingAllocator.free(key);
    }

    EXPECT_SET_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);

    int key = (int)i;
    const void *result = tlovSetFind(ints, &key);
    TLO_EXPECT(result && *(const int *)result == key);
  }

  tloSetDelete(ints);
}

void testSetIntInsertOnceRemoveOnce(TloSet *ints) {
  TLO_ASSERT(ints);

  int key = MAX_SET_SIZE;
  TloError error = tlovSetInsert(ints, &key);
  TLO_ASSERT(!error);

  bool removed = tlovSetRemove(ints, &key);
  TLO_ASSERT(removed);

  EXPECT_SET_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  const void *result = tlovSetFind(ints, &key);
  TLO_EXPECT(!result);

  tloSetDelete(ints);
}

void testSetIntInsertManyTimesRemoveUntilEmpty(TloSet *ints) {
  TLO_ASSERT(ints);

  for (size_t i = 0; i < MAX_SET_SIZE; ++i) {
    int key = (int)i;
    TloError error = tlovSetInsert(ints, &key);
    TLO_ASSERT(!error);
  }

  for (size_t i = MAX_SET_SIZE - 1; i <= MAX_SET_SIZE - 1; --i) {
    EXPECT_SET_PROPERTIES(ints, i + 1, false, &tloInt, &countingAllocator);

    int key = (int)i;
    bool removed = tlovSetRemove(ints, &key);
    TLO_ASSERT(removed);

    const void *result = tlovSetFind(ints, &key);
    TLO_EXPECT(!result);
  }

  EXPECT_SET_PROPERTIES(ints, 0, true, &tloInt, &countingAllocator);

  tloSetDelete(ints);
}
