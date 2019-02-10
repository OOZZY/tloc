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
