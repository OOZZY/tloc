#ifndef TLO_DARRAY_H
#define TLO_DARRAY_H

#include "tlo/util.h"

/*
 * - dynamic array
 */
typedef struct TloDArray {
  // private
  const TloType *type;
  const TloAllocator *allocator;
  void *bytes;
  size_t size;
  size_t capacity;
} TloDArray;

bool tloDArrayIsValid(const TloDArray *array);
tloError tloDArrayConstruct(TloDArray *array, const TloType *type,
                            const TloAllocator *allocator);
tloError tloDArrayConstructWithCapacity(TloDArray *array, const TloType *type,
                                        const TloAllocator *allocator,
                                        size_t capacity);
tloError tloDArrayConstructCopy(TloDArray *array, const TloDArray *other);
void tloDArrayDestruct(TloDArray *array);

/*
 * - allocator->malloc then tloDArrayConstruct
 */
TloDArray *tloDArrayMake(const TloType *type, const TloAllocator *allocator);

/*
 * - allocator->malloc then tloDArrayConstructWithCapacity
 */
TloDArray *tloDArrayMakeWithCapacity(const TloType *type,
                                     const TloAllocator *allocator,
                                     size_t capacity);

/*
 * - other->allocator->malloc then tloDArrayConstructCopy
 */
TloDArray *tloDArrayMakeCopy(const TloDArray *other);

/*
 * - tloDArrayDestruct then array->allocator->free
 */
void tloDArrayDelete(TloDArray *array);

tloError tloDArrayCopy(TloDArray *array, const TloDArray *other);
const TloType *tloDArrayGetType(const TloDArray *array);
const TloAllocator *tloDArrayGetAllocator(const TloDArray *array);
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
 * - deep copies data using array->type->copy
 */
tloError tloDArrayPushBack(TloDArray *array, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloDArrayMoveBack(TloDArray *array, void *data);

void tloDArrayPopBack(TloDArray *array);

#endif  // TLO_DARRAY_H
