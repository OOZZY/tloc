#ifndef TEST_UTIL_H
#define TEST_UTIL_H

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
bool intPtrEquals(const IntPtr *ptr1, const IntPtr *ptr2);
size_t intPtrHash(const IntPtr *ptr);
int intPtrCompare(const IntPtr *ptr1, const IntPtr *ptr2);
extern const TloType intPtrType;

#endif  // TEST_UTIL_H
