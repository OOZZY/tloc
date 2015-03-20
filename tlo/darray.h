#ifndef TLO_DARRAY_H
#define TLO_DARRAY_H

#include "tlo/util.h"

/*
 * * an implementation of a dynamic array
 * * all operations are transactional
 */

typedef struct tloDArray {
  const tloType *type;
  const tloAllocator *allocator;
  void *bytes;
  size_t size;
  size_t capacity;
} tloDArray;

bool tloDArrayIsValid(const tloDArray *array);
int tloDArrayConstruct(tloDArray *array, const tloType *type,
                       const tloAllocator *allocator);
int tloDArrayCopyConstruct(tloDArray *array, const tloDArray *other);
void tloDArrayDestruct(tloDArray *array);

/*
 * * allocator->malloc then tloDArrayConstruct
 */
tloDArray *tloDArrayMake(const tloType *type, const tloAllocator *allocator);

/*
 * * other->allocator->malloc then tloDArrayCopyConstruct
 */
tloDArray *tloDArrayMakeCopy(const tloDArray *other);

/*
 * * tloDArrayDestruct then array->allocator->free
 */
void tloDArrayDelete(tloDArray *array);

int tloDArrayCopy(tloDArray *array, const tloDArray *other);
const tloType *tloDArrayGetType(const tloDArray *array);
const tloAllocator *tloDArrayGetAllocator(const tloDArray *array);
size_t tloDArrayGetSize(const tloDArray *array);
size_t tloDArrayGetCapacity(const tloDArray *array);
bool tloDArrayIsEmpty(const tloDArray *array);
const void *tloDArrayGetElement(const tloDArray *array, size_t index);
void *tloDArrayGetElementReadWrite(tloDArray *array, size_t index);
const void *tloDArrayGetFront(const tloDArray *array);
void *tloDArrayGetFrontReadWrite(tloDArray *array);
const void *tloDArrayGetBack(const tloDArray *array);
void *tloDArrayGetBackReadWrite(tloDArray *array);

/*
 * * deep copies data using array->type->copy
 */
int tloDArrayPushBackCopy(tloDArray *array, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
int tloDArrayPushBackMove(tloDArray *array, void *data);

void tloDArrayPopBack(tloDArray *array);

#endif // TLO_DARRAY_H
