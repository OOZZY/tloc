#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stddef.h>
#include <stdbool.h>

/*
 * - used as return type of functions that could fail
 * - return value zero means function succeeded (no error occurred)
 * - return value non-zero means function failed (error occurred)
 */
typedef int tloError;

/*
 * - both bytes and data should not be NULL. they should point to some valid
 *   stack space or heap space allocated by some malloc implementation
 * - should do a deep copy
 */
typedef tloError (*tloConstructCopyFunction)(void *bytes, const void *data);

/*
 * - should do nothing if bytes is NULL
 * - should not fail
 */
typedef void (*tloDestructFunction)(void *bytes);

typedef struct TloType {
  // public
  size_t sizeOf;
  tloConstructCopyFunction constructCopy;
  tloDestructFunction destruct;
} TloType;

bool tloTypeIsValid(const TloType *type);

extern const TloType tloIntType;

typedef void *(*tloMallocFunction)(size_t byteCount);
typedef void (*tloFreeFunction)(void *bytes);

typedef struct TloAllocator {
  // public
  tloMallocFunction malloc;
  tloFreeFunction free;
} TloAllocator;

bool tloAllocatorIsValid(const TloAllocator *allocator);

extern const TloAllocator tloCStdLibAllocator;

extern const TloAllocator tloCountingAllocator;
unsigned long tloCountingAllocatorGetMallocCount(void);
unsigned long tloCountingAllocatorGetFreeCount(void);
unsigned long tloCountingAllocatorGetTotalByteCount(void);

#endif  // TLO_UTIL_H
