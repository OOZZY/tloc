#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include "tlo/hash.h"

/*
 * - used as return type of functions that could fail
 * - return value TLO_SUCCESS means function succeeded (no error occurred)
 * - return value TLO_ERROR means function failed (error occurred)
 * - TLO_DUPLICATE is returned by only the TloSet and TloMap insertion functions
 *   when they fail because the value or key being inserted already exists in
 *   the set or map
 */
typedef enum TloError {
  TLO_SUCCESS = 0,
  TLO_ERROR = -1,
  TLO_DUPLICATE = -2
} TloError;

typedef struct TloType {
  // public
  size_t size;

  /*
   * - both destination and source should not be NULL. they should point to some
   *   valid stack space or heap space allocated by some malloc implementation
   * - should do a deep copy
   */
  TloError (*constructCopy)(void *destination, const void *source);

  /*
   * - should do nothing if object is NULL
   * - should not fail
   */
  void (*destruct)(void *object);

  /*
   * - returns whether the objects pointed to by object1 and object2 are equal
   */
  bool (*equals)(const void *object1, const void *object2);

  TloHashFunction hash;

  /*
   * - compares the pointees of object1 and object2 for order
   * - returns <0 if pointee of object1 is less than pointee of object2
   * - returns 0 if pointee of object1 is equal to pointee of object2
   * - returns >0 if pointee of object1 is greater than pointee of object2
   */
  int (*compare)(const void *object1, const void *object2);
} TloType;

/*
 * - if type->constructCopy is not NULL, calls
 *   type->constructCopy(destination, source)
 * - otherwise, calls memcpy(destination, source, type->size);
 */
TloError tloTypeConstructCopy(const TloType *type, void *destination,
                              const void *source);

/*
 * - if type->destruct is not NULL, calls type->destruct(object);
 * - otherwise, does nothing
 */
void tloTypeDestruct(const TloType *type, void *object);

/*
 * - if type->equals is not NULL, returns type->equals(object1, object2)
 * - otherwise, returns memcmp(object1, object2, type->size) == 0
 */
bool tloTypeEquals(const TloType *type, const void *object1,
                   const void *object2);

/*
 * - if type->hash is not NULL, returns type->hash(object, type->size)
 * - otherwise, returns tloFNV1aHash(object, type->size)
 */
size_t tloTypeHash(const TloType *type, const void *object);

extern const TloType tloInt;

typedef struct TloAllocator {
  // public
  void *(*malloc)(size_t size);
  void (*free)(void *memory);
} TloAllocator;

void *tloAllocatorMallocAndZeroInitialize(const TloAllocator *allocator,
                                          size_t size);

extern const TloAllocator tloCStdLibAllocator;

void tloPtrDestruct(void *ptr);

/*
 * for containers where elements:
 * - are pointers
 * - don't need to be deep copied (constructCopy won't be used)
 * - need to be destructed by calling just free on each of them
 * - won't be compared for equality or hashed
 * - won't be compared for order
 */
extern const TloType tloPtr;

typedef char *TloCString;
extern const TloType tloCString;

#endif  // TLO_UTIL_H
