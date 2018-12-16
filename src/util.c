#include "tlo/util.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool tloTypeIsValid(const TloType *type) { return type && type->size; }

TloError tloTypeConstructCopy(const TloType *type, void *destination,
                              const void *source) {
  assert(tloTypeIsValid(type));
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
  assert(tloTypeIsValid(type));

  if (type->destruct) {
    type->destruct(object);
  }
}

const TloType tloInt = {
    .size = sizeof(int), .constructCopy = NULL, .destruct = NULL};

bool tloAllocatorIsValid(const TloAllocator *allocator) {
  return allocator && allocator->malloc && allocator->free;
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

const TloType tloPtr = {
    .size = sizeof(int *), .constructCopy = NULL, .destruct = tloPtrDestruct};
