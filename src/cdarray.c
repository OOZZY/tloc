#include "tlo/cdarray.h"
#include <assert.h>
#include <string.h>
#include "list.h"
#include "util.h"

#ifndef NDEBUG
static bool cdarrayIsValid(const TloList *list) {
  const TloCDArray *array = (const TloCDArray *)list;
  return listIsValid(list) &&
         (array->capacity == 0 ? array->front == 0
                               : array->front < array->capacity) &&
         (array->size <= array->capacity);
}
#endif

static const void *constElement(const TloCDArray *array, size_t index) {
  return array->array + ((array->front + index) % array->capacity) *
                            array->list.valueType->size;
}

static void *mutableElement(TloCDArray *array, size_t index) {
  return array->array + ((array->front + index) % array->capacity) *
                            array->list.valueType->size;
}

static void destructAllElements(TloCDArray *array) {
  for (size_t i = 0; i < array->size; ++i) {
    void *element = mutableElement(array, i);
    tloTypeDestruct(array->list.valueType, element);
  }
}

static void cdarrayDestruct(TloList *list) {
  if (!list) {
    return;
  }

  assert(cdarrayIsValid(list));

  TloCDArray *array = (TloCDArray *)list;
  if (!array->array) {
    return;
  }

  destructAllElements(array);

  array->list.allocator->free(array->array);
  array->array = NULL;
}

static size_t cdarraySize(const TloList *list) {
  assert(cdarrayIsValid(list));

  const TloCDArray *array = (const TloCDArray *)list;
  return array->size;
}

static bool cdarrayIsEmpty(const TloList *list) {
  assert(cdarrayIsValid(list));

  const TloCDArray *array = (const TloCDArray *)list;
  return array->size == 0;
}

static const void *cdarrayFront(const TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  const TloCDArray *array = (const TloCDArray *)list;
  return constElement(array, 0);
}

static void *cdarrayMutableFront(TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  TloCDArray *array = (TloCDArray *)list;
  return mutableElement(array, 0);
}

static const void *cdarrayBack(const TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  const TloCDArray *array = (const TloCDArray *)list;
  return constElement(array, array->size - 1);
}

static void *cdarrayMutableBack(TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  TloCDArray *array = (TloCDArray *)list;
  return mutableElement(array, array->size - 1);
}

enum { STARTING_CAPACITY = 1 };

static TloError allocateArrayIfNeeded(TloCDArray *array) {
  if (!array->array) {
    array->array = array->list.allocator->malloc(STARTING_CAPACITY *
                                                 array->list.valueType->size);
    if (!array->array) {
      return TLO_ERROR;
    }
    array->front = STARTING_CAPACITY / 4;
    array->capacity = STARTING_CAPACITY;
  }
  return TLO_SUCCESS;
}

static const void *constElement_(unsigned char *array, size_t index,
                                 size_t valueSize) {
  return array + index * valueSize;
}

static void *mutableElement_(unsigned char *array, size_t index,
                             size_t valueSize) {
  return array + index * valueSize;
}

static TloError expandArrayIfNeeded(TloCDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    size_t valueSize = array->list.valueType->size;
    unsigned char *newArray =
        array->list.allocator->malloc(newCapacity * valueSize);
    if (!newArray) {
      return TLO_ERROR;
    }

    size_t newFront = newCapacity / 4;
    size_t oldRightPartSize = array->size - array->front;
    size_t oldLeftPartSize = array->front;
    memcpy(mutableElement_(newArray, newFront, valueSize),
           constElement_(array->array, array->front, valueSize),
           oldRightPartSize * valueSize);
    memcpy(mutableElement_(newArray, newFront + oldRightPartSize, valueSize),
           array->array, oldLeftPartSize * valueSize);

    array->list.allocator->free(array->array);
    array->array = newArray;
    array->front = newFront;
    array->capacity = newCapacity;
  }
  return TLO_SUCCESS;
}

static TloError pushBackCopiedData(TloCDArray *array, const void *data) {
  void *destination = mutableElement(array, array->size);

  if (tloTypeConstructCopy(array->list.valueType, destination, data) ==
      TLO_ERROR) {
    return TLO_ERROR;
  }

  ++array->size;

  return TLO_SUCCESS;
}

static TloError cdarrayPushBack(TloList *list, const void *data) {
  assert(cdarrayIsValid(list));
  assert(data);

  TloCDArray *array = (TloCDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackCopiedData(array, data) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void pushBackMovedData(TloCDArray *array, void *data) {
  void *destination = mutableElement(array, array->size);

  memcpy(destination, data, array->list.valueType->size);
  array->list.allocator->free(data);

  ++array->size;
}

static TloError cdarrayMoveBack(TloList *list, void *data) {
  assert(cdarrayIsValid(list));
  assert(data);

  TloCDArray *array = (TloCDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  pushBackMovedData(array, data);

  return TLO_SUCCESS;
}

static size_t cdarrayCapacity(const TloList *list) {
  assert(cdarrayIsValid(list));

  const TloCDArray *array = (const TloCDArray *)list;
  return array->capacity;
}

static const void *cdarrayElement(const TloList *list, size_t index) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));
  const TloCDArray *array = (const TloCDArray *)list;
  assert(index < array->size);

  return constElement(array, index);
}

static void *cdarrayMutableElement(TloList *list, size_t index) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));
  TloCDArray *array = (TloCDArray *)list;
  assert(index < array->size);

  return mutableElement(array, index);
}

static size_t newFrontAfterPushFront(TloCDArray *array) {
  if (array->front == 0) {
    return array->capacity - 1;
  }
  return array->front - 1;
}

static TloError pushFrontCopiedData(TloCDArray *array, const void *data) {
  size_t newFront = newFrontAfterPushFront(array);
  size_t valueSize = array->list.valueType->size;
  void *destination = mutableElement_(array->array, newFront, valueSize);

  if (tloTypeConstructCopy(array->list.valueType, destination, data) ==
      TLO_ERROR) {
    return TLO_ERROR;
  }

  array->front = newFront;
  ++array->size;

  return TLO_SUCCESS;
}

static TloError cdarrayPushFront(TloList *list, const void *data) {
  assert(cdarrayIsValid(list));
  assert(data);

  TloCDArray *array = (TloCDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushFrontCopiedData(array, data) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void pushFrontMovedData(TloCDArray *array, void *data) {
  size_t newFront = newFrontAfterPushFront(array);
  size_t valueSize = array->list.valueType->size;
  void *destination = mutableElement_(array->array, newFront, valueSize);

  memcpy(destination, data, valueSize);
  array->list.allocator->free(data);

  array->front = newFront;
  ++array->size;
}

static TloError cdarrayMoveFront(TloList *list, void *data) {
  assert(cdarrayIsValid(list));
  assert(data);

  TloCDArray *array = (TloCDArray *)list;
  if (allocateArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(array) == TLO_ERROR) {
    return TLO_ERROR;
  }

  pushFrontMovedData(array, data);

  return TLO_SUCCESS;
}

static void cdarrayPopFront(TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  TloCDArray *array = (TloCDArray *)list;
  void *front = mutableElement(array, 0);
  tloTypeDestruct(array->list.valueType, front);
  if (array->front == array->capacity - 1) {
    array->front = 0;
  } else {
    ++array->front;
  }
  --array->size;
}

static void cdarrayPopBack(TloList *list) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));

  TloCDArray *array = (TloCDArray *)list;
  void *back = mutableElement(array, array->size - 1);
  tloTypeDestruct(array->list.valueType, back);
  --array->size;
}

static void cdarrayUnorderedRemove(TloList *list, size_t index) {
  assert(cdarrayIsValid(list));
  assert(!cdarrayIsEmpty(list));
  TloCDArray *array = (TloCDArray *)list;
  assert(index < array->size);

  if (index == array->size - 1) {
    cdarrayPopBack(list);
    return;
  }

  void *target = mutableElement(array, index);
  tloTypeDestruct(array->list.valueType, target);
  const void *back = constElement(array, array->size - 1);
  memcpy(target, back, array->list.valueType->size);
  --array->size;
}

static const TloListVTable vTable = {.type = "TloCDArray",
                                     .destruct = cdarrayDestruct,
                                     .size = cdarraySize,
                                     .isEmpty = cdarrayIsEmpty,
                                     .front = cdarrayFront,
                                     .mutableFront = cdarrayMutableFront,
                                     .back = cdarrayBack,
                                     .mutableBack = cdarrayMutableBack,
                                     .pushBack = cdarrayPushBack,
                                     .moveBack = cdarrayMoveBack,
                                     .capacity = cdarrayCapacity,
                                     .element = cdarrayElement,
                                     .mutableElement = cdarrayMutableElement,
                                     .pushFront = cdarrayPushFront,
                                     .moveFront = cdarrayMoveFront,
                                     .popFront = cdarrayPopFront,
                                     .popBack = cdarrayPopBack,
                                     .unorderedRemove = cdarrayUnorderedRemove};

TloError tloCDArrayConstruct(TloCDArray *array, const TloType *valueType,
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
  array->front = capacity / 4;
  array->size = 0;
  array->capacity = capacity;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloCDArray *array,
                                           const TloCDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = constElement(other, i);
    if (cdarrayPushBack(&array->list, element) == TLO_ERROR) {
      cdarrayDestruct(&array->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloCDArrayConstructCopy(TloCDArray *array, const TloCDArray *other) {
  assert(array);
  assert(cdarrayIsValid(&other->list));

  if (tloCDArrayConstruct(array, other->list.valueType, other->list.allocator,
                          other->capacity) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(array, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

TloCDArray *tloCDArrayMake(const TloType *valueType,
                           const TloAllocator *allocator, size_t capacity) {
  assert(typeIsValid(valueType));

  if (!allocator) {
    allocator = &tloCStdLibAllocator;
  }

  assert(allocatorIsValid(allocator));

  TloCDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloCDArrayConstruct(array, valueType, allocator, capacity) == TLO_ERROR) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

TloCDArray *tloCDArrayMakeCopy(const TloCDArray *other) {
  assert(cdarrayIsValid(&other->list));

  TloCDArray *array = other->list.allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloCDArrayConstructCopy(array, other) == TLO_ERROR) {
    other->list.allocator->free(array);
    return NULL;
  }

  return array;
}

TloError tloCDArrayCopy(TloCDArray *array, const TloCDArray *other) {
  assert(cdarrayIsValid(&array->list));
  assert(cdarrayIsValid(&other->list));

  TloCDArray copy;
  if (tloCDArrayConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  cdarrayDestruct(&array->list);
  memcpy(array, &copy, sizeof(TloCDArray));

  return TLO_SUCCESS;
}
