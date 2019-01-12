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
    if (type->constructCopy(destination, source) == TLO_ERROR) {
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

const TloType tloInt = {
    .size = sizeof(int), .constructCopy = NULL, .destruct = NULL};

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

const TloType tloPtr = {
    .size = sizeof(int *), .constructCopy = NULL, .destruct = tloPtrDestruct};

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

const TloType tloCString = {.size = sizeof(char *),
                            .constructCopy = cstringConstructCopy,
                            .destruct = tloPtrDestruct};

bool typeIsValid(const TloType *type) { return type && type->size; }

bool allocatorIsValid(const TloAllocator *allocator) {
  return allocator && allocator->malloc && allocator->free;
}
