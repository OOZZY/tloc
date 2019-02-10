#ifndef TEST_SET_TEST_UTILS_H
#define TEST_SET_TEST_UTILS_H

#include <tlo/set.h>
#include <tlo/test.h>
#include "util.h"

enum { MAX_SET_SIZE = 42 };

#define EXPECT_SET_PROPERTIES(_set, _size, _isEmpty, _keyType, _allocator) \
  do {                                                                     \
    TLO_EXPECT(tlovSetSize(_set) == (_size));                              \
    TLO_EXPECT(tlovSetIsEmpty(_set) == (_isEmpty));                        \
    TLO_EXPECT(tloSetKeyType(_set) == (_keyType));                         \
    TLO_EXPECT(tloSetAllocator(_set) == (_allocator));                     \
  } while (0)

void testSetIntInsertOnce(TloSet *ints, bool testCopy);
void testSetIntInsertManyTimes(TloSet *ints, bool testCopy);
void testSetIntInsertOnceRemoveOnce(TloSet *ints, bool testCopy);
void testSetIntInsertManyTimesRemoveUntilEmpty(TloSet *ints, bool testCopy);

#endif  // TEST_SET_TEST_UTILS_H
