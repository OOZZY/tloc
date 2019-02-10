#include "map_test_utils.h"

static int keyToValue(int key) { return key * 2; }

void testMapIntIntInsertOnce(TloMap *intsToInts, bool testCopy) {
  TLO_ASSERT(intsToInts);

  TloError error;

  if (testCopy) {
    int key = MAX_MAP_SIZE;
    int value = keyToValue(key);

    error = tlovMapInsert(intsToInts, &key, &value);
    TLO_ASSERT(!error);

    error = tlovMapInsert(intsToInts, &key, &value);
    TLO_ASSERT(error == TLO_DUPLICATE);
  } else {
    int *key = makeInt(MAX_MAP_SIZE);
    TLO_ASSERT(key);
    int *value = makeInt(keyToValue(*key));
    TLO_ASSERT(value);
    error = tlovMapMoveInsert(intsToInts, key, value);
    TLO_ASSERT(!error);

    key = makeInt(MAX_MAP_SIZE);
    TLO_ASSERT(key);
    value = makeInt(keyToValue(*key));
    TLO_ASSERT(value);
    error = tlovMapMoveInsert(intsToInts, key, value);
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
