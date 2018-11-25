#include "tlo/util.h"
#include <stdlib.h>

bool tloTypeIsValid(const TloType *type) { return type && type->sizeOf; }

const TloType tloInt = {
    .sizeOf = sizeof(int), .constructCopy = NULL, .destruct = NULL};

bool tloAllocatorTypeIsValid(const TloAllocatorType *allocatorType) {
  return allocatorType && allocatorType->malloc && allocatorType->free;
}

const TloAllocatorType tloCStdLibAllocator = {.malloc = malloc, .free = free};

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
    .sizeOf = sizeof(int *), .constructCopy = NULL, .destruct = tloPtrDestruct};
