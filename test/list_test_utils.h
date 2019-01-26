#ifndef TEST_LIST_TEST_UTILS_H
#define TEST_LIST_TEST_UTILS_H

#include <tlo/list.h>
#include <tlo/test.h>
#include "util.h"

enum { MAX_LIST_SIZE = 42 };

#define EXPECT_LIST_PROPERTIES(_list, _size, _isEmpty, _valueType, _allocator) \
  do {                                                                         \
    TLO_EXPECT(tlovListSize(_list) == (_size));                                \
    TLO_EXPECT(tlovListIsEmpty(_list) == (_isEmpty));                          \
    TLO_EXPECT(tloListValueType(_list) == (_valueType));                       \
    TLO_EXPECT(tloListAllocator(_list) == (_allocator));                       \
    if (tloListHasFunctions(_list, TLO_LIST_CAPACITY)) {                       \
      TLO_EXPECT(tlovListCapacity(_list) >= tlovListSize(_list));              \
    }                                                                          \
  } while (0)

#define EXPECT_LIST_INT_ELEMENTS(_list, _frontValue, _backValue, _index, \
                                 _indexValue)                            \
  do {                                                                   \
    TLO_EXPECT(*(const int *)tlovListFront(_list) == (_frontValue));     \
    TLO_EXPECT(*(int *)tlovListMutableFront(_list) == (_frontValue));    \
    TLO_EXPECT(*(const int *)tlovListBack(_list) == (_backValue));       \
    TLO_EXPECT(*(int *)tlovListMutableBack(_list) == (_backValue));      \
    if (tloListHasFunctions(_list, TLO_LIST_ELEMENT)) {                  \
      TLO_EXPECT(*(const int *)tlovListElement(_list, _index) ==         \
                 (_indexValue));                                         \
      TLO_EXPECT(*(int *)tlovListMutableElement(_list, _index) ==        \
                 (_indexValue));                                         \
    }                                                                    \
  } while (0)

#define EXPECT_LIST_INTPTR_ELEMENTS(_list, _frontValue, _backValue, _index,    \
                                    _indexValue)                               \
  do {                                                                         \
    TLO_EXPECT(*((const IntPtr *)tlovListFront(_list))->ptr == (_frontValue)); \
    TLO_EXPECT(*((IntPtr *)tlovListMutableFront(_list))->ptr ==                \
               (_frontValue));                                                 \
    TLO_EXPECT(*((const IntPtr *)tlovListBack(_list))->ptr == (_backValue));   \
    TLO_EXPECT(*((IntPtr *)tlovListMutableBack(_list))->ptr == (_backValue));  \
    if (tloListHasFunctions(_list, TLO_LIST_ELEMENT)) {                        \
      TLO_EXPECT(*((const IntPtr *)tlovListElement(_list, _index))->ptr ==     \
                 (_indexValue));                                               \
      TLO_EXPECT(*((IntPtr *)tlovListMutableElement(_list, _index))->ptr ==    \
                 (_indexValue));                                               \
    }                                                                          \
  } while (0)

void testListDeleteWithNull(void);
void testListHasFunctions(TloList *list, unsigned char functions);

void testListIntPushBackOnce(TloList *ints, bool testCopy);
void testListIntPushBackManyTimes(TloList *ints, bool testCopy);
void testListIntPushBackOncePopBackOnce(TloList *ints);
void testListIntPushBackManyTimesPopBackUntilEmpty(TloList *ints);
void testListIntPushFrontOnce(TloList *ints, bool testCopy);
void testListIntPushFrontManyTimes(TloList *ints, bool testCopy);
void testListIntPushFrontOncePopFrontOnce(TloList *ints);
void testListIntPushFrontManyTimesPopFrontUntilEmpty(TloList *ints);

void testListIntPtrPushBackOnce(TloList *intPtrs, bool testCopy);
void testListIntPtrPushBackManyTimes(TloList *intPtrs, bool testCopy);
void testListIntPtrPushBackOncePopBackOnce(TloList *intPtrs);
void testListIntPtrPushBackManyTimesPopBackUntilEmpty(TloList *intPtrs);
void testListIntPtrPushBackManyTimesUnorderedRemoveBackUntilEmpty(
    TloList *intPtrs);
void testListIntPtrPushBackManyTimesUnorderedRemoveFrontUntilEmpty(
    TloList *intPtrs);
void testListIntPtrPushFrontOnce(TloList *intPtrs, bool testCopy);
void testListIntPtrPushFrontManyTimes(TloList *intPtrs, bool testCopy);
void testListIntPtrPushFrontOncePopFrontOnce(TloList *intPtrs);
void testListIntPtrPushFrontManyTimesPopFrontUntilEmpty(TloList *intPtrs);

#endif  // TEST_LIST_TEST_UTILS_H
