#ifndef UTIL_H
#define UTIL_H

#include <tlo/util.h>

extern const TloAllocatorType tloCountingAllocator;
void tloCountingAllocatorResetCounts(void);
unsigned long tloCountingAllocatorMallocCount(void);
unsigned long tloCountingAllocatorFreeCount(void);
unsigned long tloCountingAllocatorTotalByteCount(void);
void tloCountingAllocatorPrintCounts(void);

typedef struct TloIntPtr {
  int *ptr;
} TloIntPtr;

TloError tloIntPtrConstruct(TloIntPtr *ptr);
TloError tloIntPtrConstructCopy(TloIntPtr *ptr, const TloIntPtr *other);
void tloPtrDestruct(void *ptr);
extern const TloType tloIntPtr;

/*
 * for containers where elements:
 * - are pointers
 * - don't need to be deep copied (constructCopy won't be used)
 * - need to be destructed by calling just free on each of them
 */
extern const TloType tloPtr;

#endif  // UTIL_H
