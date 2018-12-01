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

#endif  // LIST_TEST_H
