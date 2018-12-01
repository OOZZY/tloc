#ifndef LIST_TEST_H
#define LIST_TEST_H

#include <string.h>
#include <tlo/darray.h>
#include <tlo/list.h>

#define EXPECT_LIST_PROPERTIES(_list, _size, _isEmpty, _valueType, \
                               _allocatorType)                     \
  do {                                                             \
    TLO_EXPECT(tlovListSize(_list) == (_size));                    \
    TLO_EXPECT(tlovListIsEmpty(_list) == (_isEmpty));              \
    TLO_EXPECT(tloListValueType(_list) == (_valueType));           \
    TLO_EXPECT(tloListAllocatorType(_list) == (_allocatorType));   \
    if (strcmp(tlovListType(_list), "TloDArray") == 0) {           \
      TLO_EXPECT(tloDArrayCapacity((const TloDArray *)(_list)) >=  \
                 tlovListSize(_list));                             \
    }                                                              \
  } while (0)

#define EXPECT_LIST_INT_ELEMENTS(_list, _frontValue, _backValue, _index,    \
                                 _indexValue)                               \
  do {                                                                      \
    TLO_EXPECT(*(const int *)tlovListFront(_list) == (_frontValue));        \
    TLO_EXPECT(*(int *)tlovListMutableFront(_list) == (_frontValue));       \
    TLO_EXPECT(*(const int *)tlovListBack(_list) == (_backValue));          \
    TLO_EXPECT(*(int *)tlovListMutableBack(_list) == (_backValue));         \
    if (strcmp(tlovListType(_list), "TloDArray") == 0) {                    \
      TLO_EXPECT(*(const int *)tloDArrayElement((const TloDArray *)(_list), \
                                                _index) == (_indexValue));  \
      TLO_EXPECT(*(int *)tloDArrayMutableElement((TloDArray *)(_list),      \
                                                 _index) == (_indexValue)); \
    }                                                                       \
  } while (0)

#endif  // LIST_TEST_H
