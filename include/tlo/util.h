#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stdbool.h>
#include <stddef.h>

/*
 * - used as return type of functions that could fail
 * - return value TLO_SUCCESS means function succeeded (no error occurred)
 * - return value TLO_ERROR means function failed (error occurred)
 */
typedef enum TloError { TLO_ERROR = -1, TLO_SUCCESS = 0 } TloError;

/*
 * - both bytes and data should not be NULL. they should point to some valid
 *   stack space or heap space allocated by some malloc implementation
 * - should do a deep copy
 */
typedef TloError (*TloConstructCopyFunction)(void *bytes, const void *data);

/*
 * - should do nothing if bytes is NULL
 * - should not fail
 */
typedef void (*TloDestructFunction)(void *bytes);

typedef struct TloType {
  // public
  size_t sizeOf;
  TloConstructCopyFunction constructCopy;
  TloDestructFunction destruct;
} TloType;

bool tloTypeIsValid(const TloType *type);

extern const TloType tloInt;

typedef void *(*TloMallocFunction)(size_t byteCount);
typedef void (*TloFreeFunction)(void *bytes);

typedef struct TloAllocatorType {
  // public
  TloMallocFunction malloc;
  TloFreeFunction free;
} TloAllocatorType;

bool tloAllocatorTypeIsValid(const TloAllocatorType *allocatorType);

extern const TloAllocatorType tloCStdLibAllocator;

void tloPtrDestruct(void *ptr);

/*
 * for containers where elements:
 * - are pointers
 * - don't need to be deep copied (constructCopy won't be used)
 * - need to be destructed by calling just free on each of them
 */
extern const TloType tloPtr;

#endif  // TLO_UTIL_H
