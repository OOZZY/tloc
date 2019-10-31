#ifndef TEST_MAP_TEST_UTILS_H
#define TEST_MAP_TEST_UTILS_H

#include <tlo/map.h>
#include <tlo/test.h>
#include "util.h"

enum { MAX_MAP_SIZE = 42 };

#define EXPECT_MAP_PROPERTIES(_map, _size, _isEmpty, _keyType, _valueType, \
                              _allocator)                                  \
  do {                                                                     \
    TLO_EXPECT(tlovMapSize(_map) == (_size));                              \
    TLO_EXPECT(tlovMapIsEmpty(_map) == (_isEmpty));                        \
    TLO_EXPECT(tloMapKeyType(_map) == (_keyType));                         \
    TLO_EXPECT(tloMapValueType(_map) == (_valueType));                     \
    TLO_EXPECT(tloMapAllocator(_map) == (_allocator));                     \
  } while (0)

void testMapIntIntInsertOnce(TloMap *intsToInts, bool testCopy);
void testMapIntIntInsertManyTimes(TloMap *intsToInts, bool testCopy);
void testMapIntIntInsertOnceRemoveOnce(TloMap *intsToInts);
void testMapIntIntInsertManyTimesRemoveUntilEmpty(TloMap *intsToInts);

#endif  // TEST_MAP_TEST_UTILS_H
