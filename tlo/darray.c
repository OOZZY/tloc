#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

bool tloDArrayIsValid(const TloDArray *array) {
  return array && tloTypeIsValid(array->valueType) &&
         tloAllocatorTypeIsValid(array->allocatorType) &&
         (array->size <= array->capacity);
}

TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocatorType *allocatorType,
                            size_t capacity) {
  assert(array);
  assert(tloTypeIsValid(valueType));

  if (!allocatorType) {
    allocatorType = &tloCStdLibAllocator;
  }

  assert(tloAllocatorTypeIsValid(allocatorType));

  void *newBytes = NULL;
  if (capacity) {
    newBytes = allocatorType->malloc(capacity * valueType->sizeOf);
    if (!newBytes) {
      return TLO_ERROR;
    }
  }

  array->valueType = valueType;
  array->allocatorType = allocatorType;
  array->bytes = newBytes;
  array->size = 0;
  array->capacity = capacity;

  return TLO_SUCCESS;
}

static const void *constElement(const TloDArray *array, size_t index) {
  return (const char *)array->bytes + index * array->valueType->sizeOf;
}

static void *mutableElement(TloDArray *array, size_t index) {
  return (char *)array->bytes + index * array->valueType->sizeOf;
}

static TloError pushBackAllElementsOfOther(TloDArray *array,
                                           const TloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = constElement(other, i);
    if (tloDArrayPushBack(array, element) == TLO_ERROR) {
      tloDArrayDestruct(array);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other) {
  assert(array);
  assert(tloDArrayIsValid(other));

  if (tloDArrayConstruct(array, other->valueType, other->allocatorType,
                         other->capacity) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(array, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void destructAllElements(TloDArray *array) {
  for (size_t i = 0; i < array->size; ++i) {
    void *element = mutableElement(array, i);
    if (array->valueType->destruct) {
      array->valueType->destruct(element);
    }
  }
}

void tloDArrayDestruct(TloDArray *array) {
  if (!array) {
    return;
  }

  assert(tloDArrayIsValid(array));

  if (!array->bytes) {
    return;
  }

  destructAllElements(array);

  array->allocatorType->free(array->bytes);
  array->bytes = NULL;
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
  assert(tloDArrayIsValid(other));

  TloDArray *array = other->allocatorType->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructCopy(array, other) == TLO_ERROR) {
    other->allocatorType->free(array);
    return NULL;
  }

  return array;
}

void tloDArrayDelete(TloDArray *array) {
  if (!array) {
    return;
  }

  assert(tloDArrayIsValid(array));

  tloDArrayDestruct(array);
  TloFreeFunction free = array->allocatorType->free;
  free(array);
}

TloError tloDArrayCopy(TloDArray *array, const TloDArray *other) {
  assert(tloDArrayIsValid(array));
  assert(tloDArrayIsValid(other));

  TloDArray copy;
  if (tloDArrayConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  tloDArrayDestruct(array);
  memcpy(array, &copy, sizeof(TloDArray));

  return TLO_SUCCESS;
}

const TloType *tloDArrayValueType(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->valueType;
}

const TloAllocatorType *tloDArrayAllocatorType(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->allocatorType;
}

size_t tloDArraySize(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size;
}

size_t tloDArrayCapacity(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->capacity;
}

bool tloDArrayIsEmpty(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size == 0;
}

const void *tloDArrayElement(const TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return constElement(array, index);
}

void *tloDArrayMutableElement(TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return mutableElement(array, index);
}

const void *tloDArrayFront(const TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayElement(array, 0);
}

void *tloDArrayMutableFront(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayMutableElement(array, 0);
}

const void *tloDArrayBack(const TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayElement(array, array->size - 1);
}

void *tloDArrayMutableBack(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayMutableElement(array, array->size - 1);
}

#define STARTING_CAPACITY 2

static TloError allocateBytesIfNeeded(TloDArray *array) {
  if (!array->bytes) {
    array->bytes = array->allocatorType->malloc(STARTING_CAPACITY *
                                                array->valueType->sizeOf);
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
    void *newBytes =
        array->allocatorType->malloc(newCapacity * array->valueType->sizeOf);
    if (!newBytes) {
      return TLO_ERROR;
    }

    memcpy(newBytes, array->bytes, array->size * array->valueType->sizeOf);

    array->allocatorType->free(array->bytes);
    array->bytes = newBytes;
    array->capacity = newCapacity;
  }
  return TLO_SUCCESS;
}

static TloError pushBackCopiedData(TloDArray *array, const void *data) {
  void *destination = mutableElement(array, array->size);

  if (array->valueType->constructCopy) {
    if (array->valueType->constructCopy(destination, data) == TLO_ERROR) {
      return TLO_ERROR;
    }
  } else {
    memcpy(destination, data, array->valueType->sizeOf);
  }

  ++array->size;

  return TLO_SUCCESS;
}

TloError tloDArrayPushBack(TloDArray *array, const void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

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

  memcpy(destination, data, array->valueType->sizeOf);
  memset(data, 0, array->valueType->sizeOf);

  ++array->size;

  return TLO_SUCCESS;
}

TloError tloDArrayMoveBack(TloDArray *array, void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

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

void tloDArrayPopBack(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  void *back = mutableElement(array, array->size - 1);
  if (array->valueType->destruct) {
    array->valueType->destruct(back);
  }
  --array->size;
}

void tloDArrayUnorderedRemove(TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  if (index == array->size - 1) {
    tloDArrayPopBack(array);
    return;
  }

  void *target = mutableElement(array, index);
  if (array->valueType->destruct) {
    array->valueType->destruct(target);
  }
  void *back = mutableElement(array, array->size - 1);
  memcpy(target, back, array->valueType->sizeOf);
  --array->size;
}
