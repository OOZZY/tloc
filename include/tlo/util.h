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
 * - both destination and source should not be NULL. they should point to some
 *   valid stack space or heap space allocated by some malloc implementation
 * - should do a deep copy
 */
typedef TloError (*TloConstructCopyFunction)(void *destination,
                                             const void *source);

/*
 * - should do nothing if object is NULL
 * - should not fail
 */
typedef void (*TloDestructFunction)(void *object);

typedef struct TloType {
  // public
  size_t size;
  TloConstructCopyFunction constructCopy;
  TloDestructFunction destruct;
} TloType;

bool tloTypeIsValid(const TloType *type);

extern const TloType tloInt;

typedef void *(*TloMallocFunction)(size_t size);
typedef void (*TloFreeFunction)(void *memory);

typedef struct TloAllocator {
  // public
  TloMallocFunction malloc;
  TloFreeFunction free;
} TloAllocator;

bool tloAllocatorIsValid(const TloAllocator *allocator);

extern const TloAllocator tloCStdLibAllocator;

void tloPtrDestruct(void *ptr);

/*
 * for containers where elements:
 * - are pointers
 * - don't need to be deep copied (constructCopy won't be used)
 * - need to be destructed by calling just free on each of them
 */
extern const TloType tloPtr;

#endif  // TLO_UTIL_H
