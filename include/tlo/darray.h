#ifndef TLO_DARRAY_H
#define TLO_DARRAY_H

#include "tlo/list.h"

/*
 * - dynamic array
 */
typedef struct TloDArray {
  // public, use only for passing to tloList and tlovList functions
  TloList list;

  // private
  void *array;
  size_t size;
  size_t capacity;
} TloDArray;

TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocatorType *allocatorType,
                            size_t capacity);

/*
 * - uses TloDArray's pushBack
 */
TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other);

/*
 * - uses given allocator's malloc then tloDArrayConstruct
 */
TloDArray *tloDArrayMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType,
                         size_t capacity);

/*
 * - uses malloc of other's allocator then tloDArrayConstructCopy
 */
TloDArray *tloDArrayMakeCopy(const TloDArray *other);

/*
 * - uses tloDArrayConstructCopy and TloDArray's destruct
 */
TloError tloDArrayCopy(TloDArray *array, const TloDArray *other);

#endif  // TLO_DARRAY_H
