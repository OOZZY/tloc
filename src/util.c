#include "tlo/util.h"
#include <stdlib.h>

bool tloTypeIsValid(const TloType *type) { return type && type->size; }

const TloType tloInt = {
    .size = sizeof(int), .constructCopy = NULL, .destruct = NULL};

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
    .size = sizeof(int *), .constructCopy = NULL, .destruct = tloPtrDestruct};
