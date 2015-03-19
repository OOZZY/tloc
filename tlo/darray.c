#include "tlo/darray.h"
#include <assert.h>
#include <string.h>

bool tloDArrayIsValid(const tloDArray *array) {
  return (
    (array) &&
    (tloTypeIsValid(array->type)) &&
    (tloAllocatorIsValid(array->allocator)) &&
    ((array->bytes == NULL) == (array->size == 0)) &&
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

static const void *getElementReadOnly(const void *bytes, size_t index,
                                      size_t sizeOfElement)
{
  return (const char *)bytes + index * sizeOfElement;
}

static void *getElementReadWrite(void *bytes, size_t index,
                                 size_t sizeOfElement)
{
  return (char *)bytes + index * sizeOfElement;
}

static void destructElements(void *bytes, size_t elementCount,
                             size_t sizeOfElement,
                             tloDestructFunction destruct)
{
  for (size_t i = 0; i < elementCount; ++i) {
    void *element = getElementReadWrite(bytes, i, sizeOfElement);
    destruct(element);
  }
}

static int copyUnderlyingArray(void *newBytes, const tloDArray *other) {
  for (size_t i = 0; i < other->size; ++i) {
    void *destination = getElementReadWrite(newBytes, i, other->type->sizeOf);
    const void *source =
      getElementReadOnly(other->bytes, i, other->type->sizeOf);

    if (other->type->copyConstruct(destination, source)) {
      destructElements(newBytes, i, other->type->sizeOf, other->type->destruct);
      return 1;
    }
  }

  return 0;
}

int tloDArrayCopyConstruct(tloDArray *array, const tloDArray *other) {
  assert(array);
  assert(tloDArrayIsValid(other));

  void *newBytes =
    other->allocator->malloc(other->capacity * other->type->sizeOf);
  if (!newBytes) {
    return 1;
  }

  if (copyUnderlyingArray(newBytes, other)) {
    return 1;
  }

  array->bytes = newBytes;
  array->type = other->type;
  array->allocator = other->allocator;
  array->size = other->size;
  array->capacity = other->capacity;

  return 0;
}

static void destructAllElements(tloDArray *array) {
  destructElements(array->bytes, array->size, array->type->sizeOf,
                   array->type->destruct);
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

tloDArray *tloDArrayMakeCopy(const tloDArray *other) {
  assert(tloDArrayIsValid(other));

  tloDArray *array = other->allocator->malloc(sizeof(*array));
  if (!array) {
    return NULL;
  }

  if (tloDArrayCopyConstruct(array, other)) {
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

  void *newBytes =
    other->allocator->malloc(other->capacity * other->type->sizeOf);
  if (!newBytes) {
    return 1;
  }

  if (copyUnderlyingArray(newBytes, other)) {
    return 1;
  }

  tloDArrayDestruct(array);

  array->bytes = newBytes;
  array->type = other->type;
  array->allocator = other->allocator;
  array->size = other->size;
  array->capacity = other->capacity;

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

const void *tloDArrayGetElementReadOnly(const tloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getElementReadOnly(array->bytes, index, array->type->sizeOf);
}

void *tloDArrayGetElementReadWrite(tloDArray *array, size_t index) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));
  assert(index < array->size);

  return getElementReadWrite(array->bytes, index, array->type->sizeOf);
}

const void *tloDArrayGetFrontReadOnly(const tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementReadOnly(array, 0);
}

void *tloDArrayGetFrontReadWrite(tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementReadWrite(array, 0);
}

const void *tloDArrayGetBackReadOnly(const tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementReadOnly(array, array->size - 1);
}

void *tloDArrayGetBackReadWrite(tloDArray *array) {
  assert(tloDArrayIsValid(array));
  assert(!tloDArrayIsEmpty(array));

  return tloDArrayGetElementReadWrite(array, array->size - 1);
}

#define STARTING_CAPACITY 2

static int allocateIfNeeded(tloDArray *array) {
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

static int resizeIfNeeded(tloDArray *array) {
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

static int pushBackCopy(tloDArray *array, const void *data) {
  void *destination =
    getElementReadWrite(array->bytes, array->size, array->type->sizeOf);

  if (array->type->copyConstruct(destination, data)) {
    return 1;
  }

  ++array->size;

  return 0;
}

int tloDArrayPushBackCopy(tloDArray *array, const void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateIfNeeded(array)) {
    return 1;
  }

  if (resizeIfNeeded(array)) {
    return 1;
  }

  if (pushBackCopy(array, data)) {
    return 1;
  }

  return 0;
}

static int pushBackMove(tloDArray *array, void *data) {
  void *destination =
    getElementReadWrite(array->bytes, array->size, array->type->sizeOf);

  memcpy(destination, data, array->type->sizeOf);
  memset(data, 0, array->type->sizeOf);

  ++array->size;

  return 0;
}

int tloDArrayPushBackMove(tloDArray *array, void *data) {
  assert(tloDArrayIsValid(array));
  assert(data);

  if (allocateIfNeeded(array)) {
    return 1;
  }

  if (resizeIfNeeded(array)) {
    return 1;
  }

  if (pushBackMove(array, data)) {
    return 1;
  }

  return 0;
}
