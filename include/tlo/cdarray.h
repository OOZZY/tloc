#ifndef TLO_CDARRAY_H
#define TLO_CDARRAY_H

#include "tlo/list.h"

/*
 * - circular dynamic array
 */
typedef struct TloCDArray {
  // public, use only for passing to tloList and tlovList functions
  TloList list;

  // private
  unsigned char *array;
  size_t front;
  size_t size;
  size_t capacity;
} TloCDArray;

TloError tloCDArrayConstruct(TloCDArray *array, const TloType *valueType,
                             const TloAllocatorType *allocatorType,
                             size_t capacity);

/*
 * - uses TloCDArray's pushBack
 */
TloError tloCDArrayConstructCopy(TloCDArray *array, const TloCDArray *other);

/*
 * - uses given allocator's malloc then tloCDArrayConstruct
 */
TloCDArray *tloCDArrayMake(const TloType *valueType,
                           const TloAllocatorType *allocatorType,
                           size_t capacity);

/*
 * - uses malloc of other's allocator then tloCDArrayConstructCopy
 */
TloCDArray *tloCDArrayMakeCopy(const TloCDArray *other);

/*
 * - uses tloCDArrayConstructCopy and TloCDArray's destruct
 */
TloError tloCDArrayCopy(TloCDArray *array, const TloCDArray *other);

#endif  // TLO_CDARRAY_H
