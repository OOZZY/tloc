#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

static bool darrayIsValid(const TloList *list) {
  const TloDArray *array = (const TloDArray *)list;
  return tloListIsValid(list) && (array->size <= array->capacity);
}

static const void *constElement(const TloDArray *array, size_t index) {
  return (const char *)array->bytes + index * array->list.valueType->sizeOf;
}

static void *mutableElement(TloDArray *array, size_t index) {
  return (char *)array->bytes + index * array->list.valueType->sizeOf;
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
  if (!array->bytes) {
    return;
  }

  destructAllElements(array);

  array->list.allocatorType->free(array->bytes);
  array->bytes = NULL;
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

static TloError allocateBytesIfNeeded(TloDArray *array) {
  if (!array->bytes) {
    array->bytes = array->list.allocatorType->malloc(
        STARTING_CAPACITY * array->list.valueType->sizeOf);
    if (!array->bytes) {
      return TLO_ERROR;
    }
    array->capacity = STARTING_CAPACITY;
  }
  return TLO_SUCCESS;
}

static TloError resizeBytesIfNeeded(TloDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    void *newBytes = array->list.allocatorType->malloc(
        newCapacity * array->list.valueType->sizeOf);
    if (!newBytes) {
      return TLO_ERROR;
    }

    memcpy(newBytes, array->bytes, array->size * array->list.valueType->sizeOf);

    array->list.allocatorType->free(array->bytes);
    array->bytes = newBytes;
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
  if (allocateBytesIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (resizeBytesIfNeeded(array) == TLO_ERROR) {
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
  if (allocateBytesIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (resizeBytesIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackMovedData(array, data) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
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
                                     .moveBack = darrayMoveBack};

TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocatorType *allocatorType,
                            size_t capacity) {
  assert(array);
  assert(tloTypeIsValid(valueType));
  assert(allocatorType == NULL || tloAllocatorTypeIsValid(allocatorType));

  void *newBytes = NULL;
  if (capacity) {
    newBytes = allocatorType->malloc(capacity * valueType->sizeOf);
    if (!newBytes) {
      return TLO_ERROR;
    }
  }

  tloListConstruct(&array->list, &vTable, valueType, allocatorType);
  array->bytes = newBytes;
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

  if (!allocatorType) {
    allocatorType = &tloCStdLibAllocator;
  }

  assert(tloAllocatorTypeIsValid(allocatorType));

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

size_t tloDArrayCapacity(const TloDArray *array) {
  assert(darrayIsValid(&array->list));

  return array->capacity;
}

const void *tloDArrayElement(const TloDArray *array, size_t index) {
  assert(darrayIsValid(&array->list));
  assert(!darrayIsEmpty(&array->list));
  assert(index < array->size);

  return constElement(array, index);
}

void *tloDArrayMutableElement(TloDArray *array, size_t index) {
  assert(darrayIsValid(&array->list));
  assert(!darrayIsEmpty(&array->list));
  assert(index < array->size);

  return mutableElement(array, index);
}

void tloDArrayPopBack(TloDArray *array) {
  assert(darrayIsValid(&array->list));
  assert(!darrayIsEmpty(&array->list));

  void *back = mutableElement(array, array->size - 1);
  if (array->list.valueType->destruct) {
    array->list.valueType->destruct(back);
  }
  --array->size;
}

void tloDArrayUnorderedRemove(TloDArray *array, size_t index) {
  assert(darrayIsValid(&array->list));
  assert(!darrayIsEmpty(&array->list));
  assert(index < array->size);

  if (index == array->size - 1) {
    tloDArrayPopBack(array);
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
