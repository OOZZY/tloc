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

typedef struct tloType {
  // public
  size_t sizeOf;
  tloConstructCopyFunction constructCopy;
  tloDestructFunction destruct;
} tloType;

bool tloTypeIsValid(const tloType *type);

extern const tloType tloIntType;

typedef void *(*tloMallocFunction)(size_t byteCount);
typedef void (*tloFreeFunction)(void *bytes);

typedef struct tloAllocator {
  // public
  tloMallocFunction malloc;
  tloFreeFunction free;
} tloAllocator;

bool tloAllocatorIsValid(const tloAllocator *allocator);

extern const tloAllocator tloCStdLibAllocator;

extern const tloAllocator tloCountingAllocator;
unsigned long tloCountingAllocatorGetMallocCount(void);
unsigned long tloCountingAllocatorGetFreeCount(void);
unsigned long tloCountingAllocatorGetTotalByteCount(void);

#endif  // TLO_UTIL_H
