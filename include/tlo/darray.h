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
  void *bytes;
  size_t size;
  size_t capacity;
} TloDArray;

TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocatorType *allocatorType,
                            size_t capacity);

/*
 * - uses tloDArrayPushBack
 */
TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other);

/*
 * - allocatorType->malloc then tloDArrayConstruct
 */
TloDArray *tloDArrayMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType,
                         size_t capacity);

/*
 * - other->allocatorType->malloc then tloDArrayConstructCopy
 */
TloDArray *tloDArrayMakeCopy(const TloDArray *other);

/*
 * - uses tloDArrayConstructCopy and tloDArrayDestruct
 */
TloError tloDArrayCopy(TloDArray *array, const TloDArray *other);

size_t tloDArrayCapacity(const TloDArray *array);
const void *tloDArrayElement(const TloDArray *array, size_t index);
void *tloDArrayMutableElement(TloDArray *array, size_t index);

/*
 * - uses array->valueType->destruct if it is not NULL
 */
void tloDArrayPopBack(TloDArray *array);

/*
 * - uses array->valueType->destruct if it is not NULL
 */
void tloDArrayUnorderedRemove(TloDArray *array, size_t index);

#endif  // TLO_DARRAY_H
