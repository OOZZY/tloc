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
extern const TloType tloIntPtr;

#endif  // UTIL_H
