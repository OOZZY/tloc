#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * - used as return type of functions that could fail
 * - return value zero means function succeeded (no error occurred)
 * - return value non-zero means function failed (error occurred)
 */
typedef int TloError;

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

extern const TloAllocatorType tloCountingAllocator;
void tloCountingAllocatorResetCounts(void);
unsigned long tloCountingAllocatorGetMallocCount(void);
unsigned long tloCountingAllocatorGetFreeCount(void);
unsigned long tloCountingAllocatorGetTotalByteCount(void);
void tloCountingAllocatorPrintCounts(void);

typedef struct TloIntPtr { int *ptr; } TloIntPtr;

TloError tloIntPtrConstruct(TloIntPtr *ptr);
TloError tloIntPtrConstructCopy(TloIntPtr *ptr, const TloIntPtr *other);
void tloIntPtrDestruct(TloIntPtr *ptr);
extern const TloType tloIntPtr;

#endif  // TLO_UTIL_H
