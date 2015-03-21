#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

static const void *getElement(const void *bytes, size_t index,
                              size_t sizeOfElement)
{
  return (const char *)bytes + index * sizeOfElement;
}

static void *getElementRW(void *bytes, size_t index, size_t sizeOfElement) {
  return (char *)bytes + index * sizeOfElement;
}

static void destructElements(void *bytes, size_t elementCount,
                             size_t sizeOfElement,
                             tloDestructFunction destruct)
{
  for (size_t i = 0; i < elementCount; ++i) {
    void *element = getElementRW(bytes, i, sizeOfElement);
    destruct(element);
  }
}

static void destructAllElements(tloDArray *array) {
  destructElements(array->bytes, array->size, array->type->sizeOf,
                   array->type->destruct);
}

static int deepCopyAllElements(void *newBytes, const tloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    void *destination = getElementRW(newBytes, i, other->type->sizeOf);
    const void *source = getElement(other->bytes, i, other->type->sizeOf);

    if (other->type->constructCopy(destination, source)) {
      destructElements(newBytes, i, other->type->sizeOf, other->type->destruct);
      return 1;
    }
  }

  return 0;
}

#define STARTING_CAPACITY 2

static int allocateBytesIfNeeded(tloDArray *array) {
  if (!array->bytes) {
    array->capacity = STARTING_CAPACITY;
    array->bytes =
      array->allocator->malloc(array->capacity * array->type->sizeOf);
    if (!array->bytes) {
      return 1;
    }
  }
  return 0;
}

static int resizeBytesIfNeeded(tloDArray *array) {
  if (array->size == array->capacity) {
    size_t newCapacity = array->capacity * 2;
    void *newBytes =
      array->allocator->malloc(newCapacity * array->type->sizeOf);
    if (!newBytes) {
      return 1;
    }

    memcpy(newBytes, array->bytes, array->size * array->type->sizeOf);

    array->allocator->free(array->bytes);
    array->bytes = newBytes;
    array->capacity = newCapacity;
  }
  return 0;
}

static int pushBackCopiedData(tloDArray *array, const void *data) {
  void *destination =
    getElementRW(array->bytes, array->size, array->type->sizeOf);

  if (array->type->constructCopy(destination, data)) {
    return 1;
  }

  ++array->size;

  return 0;
}

static int pushBackMovedData(tloDArray *array, void *data) {
  void *destination =
    getElementRW(array->bytes, array->size, array->type->sizeOf);

  memcpy(destination, data, array->type->sizeOf);
  memset(data, 0, array->type->sizeOf);

  ++array->size;

  return 0;
}

bool tloDArrayIsValid(const tloDArray *array) {
  return (
    (array) &&
    (tloTypeIsValid(array->type)) &&
    (tloAllocatorIsValid(array->allocator)) &&
    (array->size <= array->capacity)
  );
}

int tloDArrayConstruct(tloDArray *array, const tloType *type,
                       const tloAllocator *allocator)
{
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

int tloDArrayConstructWithCapacity(tloDArray *array, const tloType *type,
                                  const tloAllocator *allocator,
                                  size_t capacity)
{
  assert(array);
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  void *newBytes = allocator->malloc(capacity * type->sizeOf);
  if (!newBytes) {
    return 1;
  }

  array->type = type;
  array->allocator = allocator;
  array->bytes = newBytes;
  array->size = 0;
  array->capacity = capacity;

  return 0;
}

int tloDArrayConstructCopy(tloDArray *array, const tloDArray *other) {
  assert(array);
  assert(tloDArrayIsValid(other));

  void *newBytes =
    other->allocator->malloc(other->capacity * other->type->sizeOf);
  if (!newBytes) {
    return 1;
  }

  if (deepCopyAllElements(newBytes, other)) {
    return 1;
  }

  array->bytes = newBytes;
  array->type = other->type;
  array->allocator = other->allocator;
  array->size = other->size;
  array->capacity = other->capacity;

  return 0;
}

void tloDArrayDestruct(tloDArray *array) {
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

tloDArray *tloDArrayMake(const tloType *type, const tloAllocator *allocator) {
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  tloDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstruct(array, type, allocator)) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

tloDArray *tloDArrayMakeWithCapacity(const tloType *type,
                                     const tloAllocator *allocator,
                                     size_t capacity)
{
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  tloDArray *array = allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructWithCapacity(array, type, allocator, capacity)) {
    allocator->free(array);
    return NULL;
  }

  return array;
}

tloDArray *tloDArrayMakeCopy(const tloDArray *other) {
  assert(tloDArrayIsValid(other));

  tloDArray *array = other->allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayConstructCopy(array, other)) {
    other->allocator->free(array);
    return NULL;
  }

  return array;
}

void tloDArrayDelete(tloDArray *array) {
  if (!array) {
    return;
  }

  assert(tloDArrayIsValid(array));

  tloDArrayDestruct(array);
  tloFreeFunction free = array->allocator->free;
  free(array);
}

int tloDArrayCopy(tloDArray *array, const tloDArray *other) {
  assert(tloDArrayIsValid(array));
  assert(tloDArrayIsValid(other));

  tloDArray copy;
  if (tloDArrayConstructCopy(&copy, other)) {
    return 1;
  }

  tloDArrayDestruct(array);
  memcpy(array, &copy, sizeof(tloDArray));

  return 0;
}

const tloType *tloDArrayGetType(const tloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->type;
}

const tloAllocator *tloDArrayGetAllocator(const tloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->allocator;
}

size_t tloDArrayGetSize(const tloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size;
}

size_t tloDArrayGetCapacity(const tloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->capacity;
}

bool tloDArrayIsEmpty(const tloDArray *array) {
  assert(tloDArrayIsValid(array));

  return array->size == 0;
}

const void *tloDArrayGetElement(const tloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getElement(array->bytes, index, array->type->sizeOf);
}

void *tloDArrayGetElementRW(tloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getElementRW(array->bytes, index, array->type->sizeOf);
}

const void *tloDArrayGetFront(const tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElement(array, 0);
}

void *tloDArrayGetFrontRW(tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementRW(array, 0);
}

const void *tloDArrayGetBack(const tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElement(array, array->size - 1);
}

void *tloDArrayGetBackRW(tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementRW(array, array->size - 1);
}

int tloDArrayPushBack(tloDArray *array, const void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateBytesIfNeeded(array)) {
    return 1;
  }

  if (resizeBytesIfNeeded(array)) {
    return 1;
  }

  if (pushBackCopiedData(array, data)) {
    return 1;
  }

  return 0;
}

int tloDArrayPushBackMove(tloDArray *array, void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateBytesIfNeeded(array)) {
    return 1;
  }

  if (resizeBytesIfNeeded(array)) {
    return 1;
  }

  if (pushBackMovedData(array, data)) {
    return 1;
  }

  return 0;
}

void tloDArrayPopBack(tloDArray *array) {
  assert(tloDArrayIsValid(array));

  void *back = getElementRW(array->bytes, array->size, array->type->sizeOf);
  array->type->destruct(back);
  --array->size;
}
