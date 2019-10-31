#include "map_test_utils.h"

static int keyToValue(int key) { return key * 2; }

void testMapIntIntInsertOnce(TloMap *intsToInts, bool testCopy) {
  TLO_ASSERT(intsToInts);

  TloError error;

  if (testCopy) {
    int key = MAX_MAP_SIZE;
    int value = keyToValue(key);

    error = tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
    TLO_ASSERT(!error);

    error = tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
    TLO_ASSERT(error == TLO_DUPLICATE);
  } else {
    int *key = makeInt(MAX_MAP_SIZE);
    TLO_ASSERT(key);
    int *value = makeInt(keyToValue(*key));
    TLO_ASSERT(value);
    error = tlovMapInsert(intsToInts, TLO_MOVE, key, TLO_MOVE, value);
    TLO_ASSERT(!error);

    key = makeInt(MAX_MAP_SIZE);
    TLO_ASSERT(key);
    value = makeInt(keyToValue(*key));
    TLO_ASSERT(value);
    error = tlovMapInsert(intsToInts, TLO_MOVE, key, TLO_MOVE, value);
    TLO_ASSERT(error == TLO_DUPLICATE);
    countingAllocator.free(key);
    countingAllocator.free(value);
  }

  EXPECT_MAP_PROPERTIES(intsToInts, 1, false, &tloInt, &tloInt,
                        &countingAllocator);

  int key = MAX_MAP_SIZE;
  int value = keyToValue(key);
  const void *result = tlovMapFind(intsToInts, &key);
  TLO_EXPECT(result && *(const int *)result == value);

  tloMapDelete(intsToInts);
}

void testMapIntIntInsertManyTimes(TloMap *intsToInts, bool testCopy) {
  TLO_ASSERT(intsToInts);

  for (size_t i = 0; i < MAX_MAP_SIZE; ++i) {
    TloError error;

    if (testCopy) {
      int key = (int)i;
      int value = keyToValue(key);

      error = tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
      TLO_ASSERT(!error);

      error = tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
      TLO_ASSERT(error == TLO_DUPLICATE);
    } else {
      int *key = makeInt((int)i);
      TLO_ASSERT(key);
      int *value = makeInt(keyToValue(*key));
      TLO_ASSERT(value);
      error = tlovMapInsert(intsToInts, TLO_MOVE, key, TLO_MOVE, value);
      TLO_ASSERT(!error);

      key = makeInt((int)i);
      TLO_ASSERT(key);
      value = makeInt(keyToValue(*key));
      TLO_ASSERT(value);
      error = tlovMapInsert(intsToInts, TLO_MOVE, key, TLO_MOVE, value);
      TLO_ASSERT(error == TLO_DUPLICATE);
      countingAllocator.free(key);
      countingAllocator.free(value);
    }

    EXPECT_MAP_PROPERTIES(intsToInts, i + 1, false, &tloInt, &tloInt,
                          &countingAllocator);

    int key = (int)i;
    int value = keyToValue(key);
    const void *result = tlovMapFind(intsToInts, &key);
    TLO_EXPECT(result && *(const int *)result == value);
  }

  tloMapDelete(intsToInts);
}

void testMapIntIntInsertOnceRemoveOnce(TloMap *intsToInts) {
  TLO_ASSERT(intsToInts);

  int key = MAX_MAP_SIZE;
  int value = keyToValue(key);

  TloError error = tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
  TLO_ASSERT(!error);

  bool removed = tlovMapRemove(intsToInts, &key);
  TLO_ASSERT(removed);

  EXPECT_MAP_PROPERTIES(intsToInts, 0, true, &tloInt, &tloInt,
                        &countingAllocator);

  const void *result = tlovMapFind(intsToInts, &key);
  TLO_EXPECT(!result);

  tloMapDelete(intsToInts);
}

void testMapIntIntInsertManyTimesRemoveUntilEmpty(TloMap *intsToInts) {
  TLO_ASSERT(intsToInts);

  for (size_t i = 0; i < MAX_MAP_SIZE; ++i) {
    int key = (int)i;
    int value = keyToValue(key);

    TloError error =
        tlovMapInsert(intsToInts, TLO_COPY, &key, TLO_COPY, &value);
    TLO_ASSERT(!error);
  }

  for (size_t i = MAX_MAP_SIZE - 1; i <= MAX_MAP_SIZE - 1; --i) {
    EXPECT_MAP_PROPERTIES(intsToInts, i + 1, false, &tloInt, &tloInt,
                          &countingAllocator);

    int key = (int)i;
    bool removed = tlovMapRemove(intsToInts, &key);
    TLO_ASSERT(removed);

    const void *result = tlovMapFind(intsToInts, &key);
    TLO_EXPECT(!result);
  }

  EXPECT_MAP_PROPERTIES(intsToInts, 0, true, &tloInt, &tloInt,
                        &countingAllocator);

  tloMapDelete(intsToInts);
}
