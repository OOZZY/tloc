#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

static const void *getElement(const void *bytes, size_t index,
                              size_t sizeOfElement) {
  return (const char *)bytes + index * sizeOfElement;
}

static void *getMutableElement(void *bytes, size_t index,
                               size_t sizeOfElement) {
  return (char *)bytes + index * sizeOfElement;
}

static void destructAllElements(TloDArray *array) {
  for (size_t i = 0; i < array->size; ++i) {
    void *element = getMutableElement(array->bytes, i, array->type->sizeOf);
    array->type->destruct(element);
  }
}

#define STARTING_CAPACITY 2

static tloError allocateBytesIfNeeded(TloDArray *array) {
  if (!array->bytes) {
    array->capacity = STARTING_CAPACITY;
    array->bytes =
        array->allocator->malloc(array->capacity * array->type->sizeOf);
    if (!array->bytes) {
      return -1;
    }
  }
  return 0;
}

static tloError resizeBytesIfNeeded(TloDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    void *newBytes =
        array->allocator->malloc(newCapacity * array->type->sizeOf);
    if (!newBytes) {
      return -1;
    }

    memcpy(newBytes, array->bytes, array->size * array->type->sizeOf);

    array->allocator->free(array->bytes);
    array->bytes = newBytes;
    array->capacity = newCapacity;
  }
  return 0;
}

static tloError pushBackCopiedData(TloDArray *array, const void *data) {
  void *destination =
      getMutableElement(array->bytes, array->size, array->type->sizeOf);

  if (array->type->constructCopy(destination, data) == -1) {
    return -1;
  }

  ++array->size;

  return 0;
}

static tloError pushBackMovedData(TloDArray *array, void *data) {
  void *destination =
      getMutableElement(array->bytes, array->size, array->type->sizeOf);

  memcpy(destination, data, array->type->sizeOf);
  memset(data, 0, array->type->sizeOf);

  ++array->size;

  return 0;
}

static tloError pushBackAllElementsOfOther(TloDArray *array,
                                           const TloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    const void *element = getElement(other->bytes, i, other->type->sizeOf);
    if (tloDArrayPushBack(array, element) == -1) {
      tloDArrayDestruct(array);
      return -1;
    }
  }

  return 0;
}

bool tloDArrayIsValid(const TloDArray *array) {
  return array && tloTypeIsValid(array->type) &&
         tloAllocatorIsValid(array->allocator) &&
         (array->size <= array->capacity);
}

tloError tloDArrayConstruct(TloDArray *array, const TloType *type,
                            const TloAllocator *allocator) {
  assert(array);
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  array->type = type;
  array->allocator = allocator;
  array->bytes = NULL;
  array->size = 0;
  array->capacity = 0;

  return 0;
}

tloError tloDArrayConstructWithCapacity(TloDArray *array, const TloType *type,
                                        const TloAllocator *allocator,
                                        size_t capacity) {
  assert(array);
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));
  assert(capacity);

  void *newBytes = allocator->malloc(capacity * type->sizeOf);
  if (!newBytes) {
    return -1;
  }

  array->type = type;
  array->allocator = allocator;
  array->bytes = newBytes;
  array->size = 0;
  array->capacity = capacity;

  return 0;
}

tloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other) {
  assert(array);
  assert(tloDArrayIsValid(other));

  if (tloDArrayConstructWithCapacity(array, other->type, other->allocator,
                                     other->capacity) == -1) {
    return -1;
  }

  if (pushBackAllElementsOfOther(array, other) == -1) {
    return -1;
  }

  return 0;
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

  array->allocator->free(array->bytes);
  array->bytes = NULL;
}

TloDArray *tloDArrayMake(const TloType *type, const TloAllocator *allocator) {
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  TloDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstruct(array, type, allocator)) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

TloDArray *tloDArrayMakeWithCapacity(const TloType *type,
                                     const TloAllocator *allocator,
                                     size_t capacity) {
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));
  assert(capacity);

  TloDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructWithCapacity(array, type, allocator, capacity)) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

TloDArray *tloDArrayMakeCopy(const TloDArray *other) {
  assert(tloDArrayIsValid(other));

  TloDArray *array = other->allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructCopy(array, other)) {
    other->allocator->free(array);
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
  tloFreeFunction free = array->allocator->free;
  free(array);
}

tloError tloDArrayCopy(TloDArray *array, const TloDArray *other) {
  assert(tloDArrayIsValid(array));
  assert(tloDArrayIsValid(other));

  TloDArray copy;
  if (tloDArrayConstructCopy(&copy, other) == -1) {
    return -1;
  }

  tloDArrayDestruct(array);
  memcpy(array, &copy, sizeof(TloDArray));

  return 0;
}

const TloType *tloDArrayGetType(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->type;
}

const TloAllocator *tloDArrayGetAllocator(const TloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->allocator;
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

  return getElement(array->bytes, index, array->type->sizeOf);
}

void *tloDArrayGetMutableElement(TloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getMutableElement(array->bytes, index, array->type->sizeOf);
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

tloError tloDArrayPushBack(TloDArray *array, const void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateBytesIfNeeded(array) == -1) {
    return -1;
  }

  if (resizeBytesIfNeeded(array) == -1) {
    return -1;
  }

  if (pushBackCopiedData(array, data) == -1) {
    return -1;
  }

  return 0;
}

tloError tloDArrayMoveBack(TloDArray *array, void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateBytesIfNeeded(array) == -1) {
    return -1;
  }

  if (resizeBytesIfNeeded(array) == -1) {
    return -1;
  }

  if (pushBackMovedData(array, data) == -1) {
    return -1;
  }

  return 0;
}

void tloDArrayPopBack(TloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  void *back =
      getMutableElement(array->bytes, array->size, array->type->sizeOf);
  array->type->destruct(back);
  --array->size;
}
