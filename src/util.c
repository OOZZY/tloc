#include "util.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

TloError tloTypeConstructCopy(const TloType *type, void *destination,
                              const void *source) {
  assert(typeIsValid(type));
  assert(destination);
  assert(source);

  if (type->constructCopy) {
    if (type->constructCopy(destination, source) != TLO_SUCCESS) {
      return TLO_ERROR;
    }
  } else {
    memcpy(destination, source, type->size);
  }

  return TLO_SUCCESS;
}

void tloTypeDestruct(const TloType *type, void *object) {
  assert(typeIsValid(type));

  if (type->destruct) {
    type->destruct(object);
  }
}

bool tloTypeEquals(const TloType *type, const void *object1,
                   const void *object2) {
  assert(typeIsValid(type));
  assert(object1);
  assert(object2);

  if (type->equals) {
    return type->equals(object1, object2);
  }

  return memcmp(object1, object2, type->size) == 0;
}

size_t tloTypeHash(const TloType *type, const void *object) {
  assert(typeIsValid(type));
  assert(object);

  if (type->hash) {
    return type->hash(object, type->size);
  }

  return tloFNV1aHash(object, type->size);
}

static int intCompare(const void *object1, const void *object2) {
  assert(object1);
  assert(object2);

  const int *int1 = object1;
  const int *int2 = object2;

  if (*int1 < *int2) {
    return -1;
  } else if (*int1 > *int2) {
    return 1;
  } else {
    return 0;
  }
}

const TloType tloInt = {.size = sizeof(int), .compare = intCompare};

void *tloAllocatorMallocAndZeroInitialize(const TloAllocator *allocator,
                                          size_t size) {
  assert(allocatorIsValid(allocator));

  void *memory = allocator->malloc(size);
  if (!memory) {
    return NULL;
  }

  memset(memory, 0, size);
  return memory;
}

const TloAllocator tloCStdLibAllocator = {.malloc = malloc, .free = free};

void tloPtrDestruct(void *ptr) {
  int **ptrptr = ptr;

  if (!ptrptr) {
    return;
  }

  if (!*ptrptr) {
    return;
  }

  free(*ptrptr);
  *ptrptr = NULL;
}

const TloType tloPtr = {.size = sizeof(int *), .destruct = tloPtrDestruct};

static TloError cstringConstructCopy(void *destination, const void *source) {
  assert(destination);
  assert(source);

  TloCString *destinationCString = destination;
  const TloCString *sourceCString = source;

  size_t size = strlen(*sourceCString);
  *destinationCString = malloc(size + 1);
  if (!*destinationCString) {
    return TLO_ERROR;
  }

  strcpy(*destinationCString, *sourceCString);
  return TLO_SUCCESS;
}

static bool cstringEquals(const void *object1, const void *object2) {
  assert(object1);
  assert(object2);

  const TloCString *cstring1 = object1;
  const TloCString *cstring2 = object2;
  return strcmp(*cstring1, *cstring2) == 0;
}

static size_t cstringHash(const void *data, size_t size) {
  assert(data);

  const TloCString *cstring = data;
  size = strlen(*cstring);
  return tloFNV1aHash(*cstring, size);
}

static int cstringCompare(const void *object1, const void *object2) {
  assert(object1);
  assert(object2);

  const TloCString *cstring1 = object1;
  const TloCString *cstring2 = object2;
  return strcmp(*cstring1, *cstring2);
}

const TloType tloCString = {.size = sizeof(char *),
                            .constructCopy = cstringConstructCopy,
                            .destruct = tloPtrDestruct,
                            .equals = cstringEquals,
                            .hash = cstringHash,
                            .compare = cstringCompare};

bool typeIsValid(const TloType *type) { return type && type->size; }

bool allocatorIsValid(const TloAllocator *allocator) {
  return allocator && allocator->malloc && allocator->free;
}
