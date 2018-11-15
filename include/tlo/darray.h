#ifndef TLO_DARRAY_H
#define TLO_DARRAY_H

#include "tlo/util.h"

/*
 * - dynamic array
 */
typedef struct TloDArray {
  // private
  const TloType *valueType;
  const TloAllocatorType *allocatorType;
  void *bytes;
  size_t size;
  size_t capacity;
} TloDArray;

bool tloDArrayIsValid(const TloDArray *array);
TloError tloDArrayConstruct(TloDArray *array, const TloType *valueType,
                            const TloAllocatorType *allocatorType,
                            size_t capacity);

/*
 * - uses tloDArrayPushBack
 */
TloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other);

/*
 * - uses array->valueType->destruct if it is not NULL
 */
void tloDArrayDestruct(TloDArray *array);

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
 * - tloDArrayDestruct then array->allocatorType->free
 */
void tloDArrayDelete(TloDArray *array);

/*
 * - uses tloDArrayConstructCopy and tloDArrayDestruct
 */
TloError tloDArrayCopy(TloDArray *array, const TloDArray *other);

const TloType *tloDArrayValueType(const TloDArray *array);
const TloAllocatorType *tloDArrayAllocatorType(const TloDArray *array);
size_t tloDArraySize(const TloDArray *array);
size_t tloDArrayCapacity(const TloDArray *array);
bool tloDArrayIsEmpty(const TloDArray *array);
const void *tloDArrayElement(const TloDArray *array, size_t index);
void *tloDArrayMutableElement(TloDArray *array, size_t index);
const void *tloDArrayFront(const TloDArray *array);
void *tloDArrayMutableFront(TloDArray *array);
const void *tloDArrayBack(const TloDArray *array);
void *tloDArrayMutableBack(TloDArray *array);

/*
 * - deep copies data using array->valueType->constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tloDArrayPushBack(TloDArray *array, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
TloError tloDArrayMoveBack(TloDArray *array, void *data);

/*
 * - uses array->valueType->destruct if it is not NULL
 */
void tloDArrayPopBack(TloDArray *array);

/*
 * - uses array->valueType->destruct if it is not NULL
 */
void tloDArrayUnorderedRemove(TloDArray *array, size_t index);

#endif  // TLO_DARRAY_H