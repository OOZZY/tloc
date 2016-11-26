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

static const void *getElement(const void *bytes, size_t index,
                              size_t sizeOfElement) {
  return (const char *)bytes + index * sizeOfElement;
}

static void *getMutableElement(void *bytes, size_t index,
                               size_t sizeOfElement) {
  return (char *)bytes + index * sizeOfElement;
}

static TloError pushBackAllElementsOfOther(TloDArray *array,
                                           const TloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = getElement(other->bytes, i, other->valueType->sizeOf);
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
    void *element =
        getMutableElement(array->bytes, i, array->valueType->sizeOf);
    array->valueType->destruct(element);
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

const TloType *tloDArrayGetValueType(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->valueType;
}

const TloAllocatorType *tloDArrayGetAllocatorType(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->allocatorType;
}

size_t tloDArrayGetSize(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size;
}

size_t tloDArrayGetCapacity(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->capacity;
}

bool tloDArrayIsEmpty(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size == 0;
}

const void *tloDArrayGetElement(const TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getElement(array->bytes, index, array->valueType->sizeOf);
}

void *tloDArrayGetMutableElement(TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getMutableElement(array->bytes, index, array->valueType->sizeOf);
}

const void *tloDArrayGetFront(const TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElement(array, 0);
}

void *tloDArrayGetMutableFront(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetMutableElement(array, 0);
}

const void *tloDArrayGetBack(const TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElement(array, array->size - 1);
}

void *tloDArrayGetMutableBack(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetMutableElement(array, array->size - 1);
}

#define STARTING_CAPACITY 2

static TloError allocateBytesIfNeeded(TloDArray *array) {
  if (!array->bytes) {
    array->capacity = STARTING_CAPACITY;
    array->bytes = array->allocatorType->malloc(array->capacity *
                                                array->valueType->sizeOf);
    if (!array->bytes) {
      return TLO_ERROR;
    }
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
  void *destination =
      getMutableElement(array->bytes, array->size, array->valueType->sizeOf);

  if (array->valueType->constructCopy(destination, data) == TLO_ERROR) {
    return TLO_ERROR;
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
  void *destination =
      getMutableElement(array->bytes, array->size, array->valueType->sizeOf);

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

  void *back = getMutableElement(array->bytes, array->size - 1,
                                 array->valueType->sizeOf);
  array->valueType->destruct(back);
  --array->size;
}
