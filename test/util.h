#ifndef UTIL_H
#define UTIL_H

#include <tlo/util.h>

extern const TloAllocator countingAllocator;
void countingAllocatorResetCounts(void);
unsigned long countingAllocatorMallocCount(void);
unsigned long countingAllocatorFreeCount(void);
unsigned long countingAllocatorTotalByteCount(void);
void countingAllocatorPrintCounts(void);

int *makeInt(int value);

typedef struct IntPtr {
  int *ptr;
} IntPtr;

TloError intPtrConstruct(IntPtr *ptr, int value);
TloError intPtrConstructCopy(IntPtr *ptr, const IntPtr *other);
IntPtr *intPtrMake(int value);
extern const TloType intPtrType;

#endif  // UTIL_H
