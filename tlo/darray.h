#ifndef TLO_DARRAY_H
#define TLO_DARRAY_H

#include "tlo/util.h"

/*
 * - dynamic array
 */

typedef struct tloDArray {
  // private
  const tloType *type;
  const tloAllocator *allocator;
  void *bytes;
  size_t size;
  size_t capacity;
} tloDArray;

bool tloDArrayIsValid(const tloDArray *array);
tloError tloDArrayConstruct(tloDArray *array, const tloType *type,
                            const tloAllocator *allocator);
tloError tloDArrayConstructWithCapacity(tloDArray *array, const tloType *type,
                                        const tloAllocator *allocator,
                                        size_t capacity);
tloError tloDArrayConstructCopy(tloDArray *array, const tloDArray *other);
void tloDArrayDestruct(tloDArray *array);

/*
 * - allocator->malloc then tloDArrayConstruct
 */
tloDArray *tloDArrayMake(const tloType *type, const tloAllocator *allocator);

/*
 * - allocator->malloc then tloDArrayConstructWithCapacity
 */
tloDArray *tloDArrayMakeWithCapacity(const tloType *type,
                                     const tloAllocator *allocator,
                                     size_t capacity);

/*
 * - other->allocator->malloc then tloDArrayConstructCopy
 */
tloDArray *tloDArrayMakeCopy(const tloDArray *other);

/*
 * - tloDArrayDestruct then array->allocator->free
 */
void tloDArrayDelete(tloDArray *array);

tloError tloDArrayCopy(tloDArray *array, const tloDArray *other);
const tloType *tloDArrayGetType(const tloDArray *array);
const tloAllocator *tloDArrayGetAllocator(const tloDArray *array);
size_t tloDArrayGetSize(const tloDArray *array);
size_t tloDArrayGetCapacity(const tloDArray *array);
bool tloDArrayIsEmpty(const tloDArray *array);
const void *tloDArrayGetElement(const tloDArray *array, size_t index);
void *tloDArrayGetMutableElement(tloDArray *array, size_t index);
const void *tloDArrayGetFront(const tloDArray *array);
void *tloDArrayGetMutableFront(tloDArray *array);
const void *tloDArrayGetBack(const tloDArray *array);
void *tloDArrayGetMutableBack(tloDArray *array);

/*
 * - deep copies data using array->type->copy
 */
tloError tloDArrayPushBack(tloDArray *array, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloDArrayMoveBack(tloDArray *array, void *data);

void tloDArrayPopBack(tloDArray *array);

#endif  // TLO_DARRAY_H
