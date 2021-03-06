#include "tlo/darray.h"
#include <assert.h>
#include <string.h>
#include "list.h"
#include "util.h"

#ifndef NDEBUG
static bool darrayIsValid(const TloList *list) {
  const TloDArray *array = (const TloDArray *)list;
  return listIsValid(list) && (array->size <= array->capacity);
}
#endif

static const void *constElement(const TloDArray *array, size_t index) {
  return array->array + index * array->list.valueType->size;
}

static void *mutableElement(TloDArray *array, size_t index) {
  return array->array + index * array->list.valueType->size;
}

static void destructAllElements(TloDArray *array) {
  for (size_t i = 0; i < array->size; ++i) {
    void *element = mutableElement(array, i);
    tloTypeDestruct(array->list.valueType, element);
  }
}

static void darrayDestruct(TloList *list) {
  if (!list) {
    return;
  }

  assert(darrayIsValid(list));

  TloDArray *array = (TloDArray *)list;
  if (!array->array) {
    return;
  }

  destructAllElements(array);

  array->list.allocator->free(array->array);
  array->array = NULL;
}

static size_t darraySize(const TloList *list) {
  assert(darrayIsValid(list));

  const TloDArray *array = (const TloDArray *)list;
  return array->size;
}

static bool darrayIsEmpty(const TloList *list) {
  assert(darrayIsValid(list));

  const TloDArray *array = (const TloDArray *)list;
  return array->size == 0;
}

static const void *darrayFront(const TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  const TloDArray *array = (const TloDArray *)list;
  return constElement(array, 0);
}

static void *darrayMutableFront(TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  TloDArray *array = (TloDArray *)list;
  return mutableElement(array, 0);
}

static const void *darrayBack(const TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  const TloDArray *array = (const TloDArray *)list;
  return constElement(array, array->size - 1);
}

static void *darrayMutableBack(TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  TloDArray *array = (TloDArray *)list;
  return mutableElement(array, array->size - 1);
}

enum { STARTING_CAPACITY = 1 };

static TloError allocateArrayIfNeeded(TloDArray *array) {
  if (!array->array) {
    array->array = array->list.allocator->malloc(STARTING_CAPACITY *
                                                 array->list.valueType->size);
    if (!array->array) {
      return TLO_ERROR;
    }
    array->capacity = STARTING_CAPACITY;
  }
  return TLO_SUCCESS;
}

static TloError expandArrayIfNeeded(TloDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    unsigned char *newArray = array->list.allocator->malloc(
        newCapacity * array->list.valueType->size);
    if (!newArray) {
      return TLO_ERROR;
    }

    memcpy(newArray, array->array, array->size * array->list.valueType->size);

    array->list.allocator->free(array->array);
    array->array = newArray;
    array->capacity = newCapacity;
  }
  return TLO_SUCCESS;
}

static TloError pushBackCopiedData(TloDArray *array, const void *data) {
  void *destination = mutableElement(array, array->size);

  if (tloTypeConstructCopy(array->list.valueType, destination, data) !=
      TLO_SUCCESS) {
    return TLO_ERROR;
  }

  ++array->size;

  return TLO_SUCCESS;
}

static TloError darrayPushBack(TloList *list, const void *data) {
  assert(darrayIsValid(list));
  assert(data);

  TloDArray *array = (TloDArray *)list;
  if (allocateArrayIfNeeded(array) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  if (pushBackCopiedData(array, data) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void pushBackMovedData(TloDArray *array, void *data) {
  void *destination = mutableElement(array, array->size);

  memcpy(destination, data, array->list.valueType->size);
  array->list.allocator->free(data);

  ++array->size;
}

static TloError darrayMoveBack(TloList *list, void *data) {
  assert(darrayIsValid(list));
  assert(data);

  TloDArray *array = (TloDArray *)list;
  if (allocateArrayIfNeeded(array) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  pushBackMovedData(array, data);

  return TLO_SUCCESS;
}

static size_t darrayCapacity(const TloList *list) {
  assert(darrayIsValid(list));

  const TloDArray *array = (const TloDArray *)list;
  return array->capacity;
}

static const void *darrayElement(const TloList *list, size_t index) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));
  const TloDArray *array = (const TloDArray *)list;
  assert(index < array->size);

  return constElement(array, index);
}

static void *darrayMutableElement(TloList *list, size_t index) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));
  TloDArray *array = (TloDArray *)list;
  assert(index < array->size);

  return mutableElement(array, index);
}

// if allocation of smaller array fails, just returns without reporting any
// error
static void shrinkArrayIfNeeded(TloDArray *array) {
  if (array->size <= array->capacity / 4 && array->size) {
    size_t newCapacity = array->capacity / 2;
    unsigned char *newArray = array->list.allocator->malloc(
        newCapacity * array->list.valueType->size);
    if (!newArray) {
      return;
    }

    memcpy(newArray, array->array, array->size * array->list.valueType->size);

    array->list.allocator->free(array->array);
    array->array = newArray;
    array->capacity = newCapacity;
  }
}

static void darrayPopBack(TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  TloDArray *array = (TloDArray *)list;
  void *back = mutableElement(array, array->size - 1);
  tloTypeDestruct(array->list.valueType, back);
  --array->size;
  shrinkArrayIfNeeded(array);
}

static void darrayUnorderedRemove(TloList *list, size_t index) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));
  TloDArray *array = (TloDArray *)list;
  assert(index < array->size);

  if (index == array->size - 1) {
    darrayPopBack(list);
    return;
  }

  void *target = mutableElement(array, index);
  tloTypeDestruct(array->list.valueType, target);
  const void *back = constElement(array, array->size - 1);
  memcpy(target, back, array->list.valueType->size);
  --array->size;
  shrinkArrayIfNeeded(array);
}

static const TloListVTable vTable = {.type = "TloDArray",
                                     .destruct = darrayDestruct,
                                     .size = darraySize,
                                     .isEmpty = darrayIsEmpty,
                                     .front = darrayFront,
                                     .mutableFront = darrayMutableFront,
                                     .back = darrayBack,
                                     .mutableBack = darrayMutableBack,
                                     .pushBack = darrayPushBack,
                                     .moveBack = darrayMoveBack,
                                     .capacity = darrayCapacity,
                                     .element = darrayElement,
                                     .mutableElement = darrayMutableElement,
                                     .popBack = darrayPopBack,
                                     .unorderedRemove = darrayUnorderedRemove};

TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocator *allocator, size_t capacity) {
  assert(array);
  assert(typeIsValid(valueType));
  assert(allocator == NULL || allocatorIsValid(allocator));

  unsigned char *newArray = NULL;
  if (capacity) {
    newArray = allocator->malloc(capacity * valueType->size);
    if (!newArray) {
      return TLO_ERROR;
    }
  }

  tloListConstruct(&array->list, &vTable, valueType, allocator);
  array->array = newArray;
  array->size = 0;
  array->capacity = capacity;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloDArray *array,
                                           const TloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = constElement(other, i);
    if (darrayPushBack(&array->list, element) != TLO_SUCCESS) {
      darrayDestruct(&array->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other) {
  assert(array);
  assert(darrayIsValid(&other->list));

  if (tloDArrayConstruct(array, other->list.valueType, other->list.allocator,
                         other->capacity) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(array, other) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

TloDArray *tloDArrayMake(const TloType *valueType,
                         const TloAllocator *allocator, size_t capacity) {
  assert(typeIsValid(valueType));

  if (!allocator) {
    allocator = &tloCStdLibAllocator;
  }

  assert(allocatorIsValid(allocator));

  TloDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstruct(array, valueType, allocator, capacity) !=
      TLO_SUCCESS) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

TloDArray *tloDArrayMakeCopy(const TloDArray *other) {
  assert(darrayIsValid(&other->list));

  TloDArray *array = other->list.allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructCopy(array, other) != TLO_SUCCESS) {
    other->list.allocator->free(array);
    return NULL;
  }

  return array;
}

TloError tloDArrayCopy(TloDArray *array, const TloDArray *other) {
  assert(darrayIsValid(&array->list));
  assert(darrayIsValid(&other->list));

  TloDArray copy;
  if (tloDArrayConstructCopy(&copy, other) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  darrayDestruct(&array->list);
  memcpy(array, &copy, sizeof(TloDArray));

  return TLO_SUCCESS;
}
