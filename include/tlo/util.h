#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stdbool.h>
#include <stddef.h>
#include "tlo/hash.h"

/*
 * - used as return type of functions that could fail
 * - return value TLO_SUCCESS means function succeeded (no error occurred)
 * - return value TLO_ERROR means function failed (error occurred)
 */
typedef enum TloError { TLO_ERROR = -1, TLO_SUCCESS = 0 } TloError;

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

extern const TloAllocator tloCStdLibAllocator;

void tloPtrDestruct(void *ptr);

/*
 * for containers where elements:
 * - are pointers
 * - don't need to be deep copied (constructCopy won't be used)
 * - need to be destructed by calling just free on each of them
 * - won't be compared for equality or hashed
 */
extern const TloType tloPtr;

typedef char *TloCString;
extern const TloType tloCString;

#endif  // TLO_UTIL_H
