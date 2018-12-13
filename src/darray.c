#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

static bool darrayIsValid(const TloList *list) {
  const TloDArray *array = (const TloDArray *)list;
  return tloListIsValid(list) && (array->size <= array->capacity);
}

static const void *constElement(const TloDArray *array, size_t index) {
  return (const char *)array->array + index * array->list.valueType->sizeOf;
}

static void *mutableElement(TloDArray *array, size_t index) {
  return (char *)array->array + index * array->list.valueType->sizeOf;
}

static void destructAllElements(TloDArray *array) {
  for (size_t i = 0; i < array->size; ++i) {
    void *element = mutableElement(array, i);
    if (array->list.valueType->destruct) {
      array->list.valueType->destruct(element);
    }
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

  array->list.allocatorType->free(array->array);
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

#define STARTING_CAPACITY 1

static TloError allocateArrayIfNeeded(TloDArray *array) {
  if (!array->array) {
    array->array = array->list.allocatorType->malloc(
        STARTING_CAPACITY * array->list.valueType->sizeOf);
    if (!array->array) {
      return TLO_ERROR;
    }
    array->capacity = STARTING_CAPACITY;
  }
  return TLO_SUCCESS;
}

static TloError resizeArrayIfNeeded(TloDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    void *newArray = array->list.allocatorType->malloc(
        newCapacity * array->list.valueType->sizeOf);
    if (!newArray) {
      return TLO_ERROR;
    }

    memcpy(newArray, array->array, array->size * array->list.valueType->sizeOf);

    array->list.allocatorType->free(array->array);
    array->array = newArray;
    array->capacity = newCapacity;
  }
  return TLO_SUCCESS;
}

static TloError pushBackCopiedData(TloDArray *array, const void *data) {
  void *destination = mutableElement(array, array->size);

  if (array->list.valueType->constructCopy) {
    if (array->list.valueType->constructCopy(destination, data) == TLO_ERROR) {
      return TLO_ERROR;
    }
  } else {
    memcpy(destination, data, array->list.valueType->sizeOf);
  }

  ++array->size;

  return TLO_SUCCESS;
}

static TloError darrayPushBack(TloList *list, const void *data) {
  assert(darrayIsValid(list));
  assert(data);

  TloDArray *array = (TloDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (resizeArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackCopiedData(array, data) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static TloError pushBackMovedData(TloDArray *array, void *data) {
  void *destination = mutableElement(array, array->size);

  memcpy(destination, data, array->list.valueType->sizeOf);
  array->list.allocatorType->free(data);

  ++array->size;

  return TLO_SUCCESS;
}

static TloError darrayMoveBack(TloList *list, void *data) {
  assert(darrayIsValid(list));
  assert(data);

  TloDArray *array = (TloDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (resizeArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackMovedData(array, data) == TLO_ERROR) {
    return TLO_ERROR;
  }

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

static void darrayPopBack(TloList *list) {
  assert(darrayIsValid(list));
  assert(!darrayIsEmpty(list));

  TloDArray *array = (TloDArray *)list;
  void *back = mutableElement(array, array->size - 1);
  if (array->list.valueType->destruct) {
    array->list.valueType->destruct(back);
  }
  --array->size;
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
  if (array->list.valueType->destruct) {
    array->list.valueType->destruct(target);
  }
  void *back = mutableElement(array, array->size - 1);
  memcpy(target, back, array->list.valueType->sizeOf);
  --array->size;
}

static const TloListVTable vTable = {.type = "TloDArray",
                                     .isValid = darrayIsValid,
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
                            const TloAllocatorType *allocatorType,
                            size_t capacity) {
  assert(array);
  assert(tloTypeIsValid(valueType));
  assert(allocatorType == NULL || tloAllocatorTypeIsValid(allocatorType));

  void *newArray = NULL;
  if (capacity) {
    newArray = allocatorType->malloc(capacity * valueType->sizeOf);
    if (!newArray) {
      return TLO_ERROR;
    }
  }

  tloListConstruct(&array->list, &vTable, valueType, allocatorType);
  array->array = newArray;
  array->size = 0;
  array->capacity = capacity;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloDArray *array,
                                           const TloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = constElement(other, i);
    if (darrayPushBack(&array->list, element) == TLO_ERROR) {
      darrayDestruct(&array->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other) {
  assert(array);
  assert(darrayIsValid(&other->list));

  if (tloDArrayConstruct(array, other->list.valueType,
                         other->list.allocatorType,
                         other->capacity) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(array, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

TloDArray *tloDArrayMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType,
                         size_t capacity) {
  assert(tloTypeIsValid(valueType));
  assert(allocatorType == NULL || tloAllocatorTypeIsValid(allocatorType));

  TloDArray *array = allocatorType->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstruct(array, valueType, allocatorType, capacity) ==
      TLO_ERROR) {
    allocatorType->free(array);
    return NULL;
  }

  return array;
}

TloDArray *tloDArrayMakeCopy(const TloDArray *other) {
  assert(darrayIsValid(&other->list));

  TloDArray *array = other->list.allocatorType->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructCopy(array, other) == TLO_ERROR) {
    other->list.allocatorType->free(array);
    return NULL;
  }

  return array;
}

TloError tloDArrayCopy(TloDArray *array, const TloDArray *other) {
  assert(darrayIsValid(&array->list));
  assert(darrayIsValid(&other->list));

  TloDArray copy;
  if (tloDArrayConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  darrayDestruct(&array->list);
  memcpy(array, &copy, sizeof(TloDArray));

  return TLO_SUCCESS;
}
