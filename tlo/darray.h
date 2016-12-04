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
 * - deep copies data using other->valueType->constructCopy
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

TloError tloDArrayCopy(TloDArray *array, const TloDArray *other);
const TloType *tloDArrayGetValueType(const TloDArray *array);
const TloAllocatorType *tloDArrayGetAllocatorType(const TloDArray *array);
size_t tloDArrayGetSize(const TloDArray *array);
size_t tloDArrayGetCapacity(const TloDArray *array);
bool tloDArrayIsEmpty(const TloDArray *array);
const void *tloDArrayGetElement(const TloDArray *array, size_t index);
void *tloDArrayGetMutableElement(TloDArray *array, size_t index);
const void *tloDArrayGetFront(const TloDArray *array);
void *tloDArrayGetMutableFront(TloDArray *array);
const void *tloDArrayGetBack(const TloDArray *array);
void *tloDArrayGetMutableBack(TloDArray *array);

/*
 * - deep copies data using array->valueType->constructCopy
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
