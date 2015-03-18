#ifndef TLO_UTIL_H
#define TLO_UTIL_H

#include <stddef.h>
#include <stdbool.h>

/*
 * * some utilities including:
 *   * typedefs of function pointer types
 *   * a struct for storing type information
 *     * a few const objects of this type storing the type information of some
 *       types
 *   * a struct for storing allocator information
 *     * two const objects of this type storing the allocator information of
 *       two allocators. one using the c standard library's malloc and free.
 *       another one using custom implementations of malloc of free (which
 *       actually just wrap around the c standard library's malloc and free)
 * * all operations are transactional
 */

/*
 * * both obj and data should not be NULL. they should point to some valid
 *   space such as the following:
 *   * heap space allocated by some malloc implementation
 *   * stack space
 * * should do a deep copy
 */
typedef int (*tloConstructFunction)(void *obj, const void *data);

/*
 * * does nothing if obj is NULL
 * * should not fail
 */
typedef void (*tloDestructFunction)(void *obj);


typedef struct tloType {
  size_t sizeOf;
  tloConstructFunction copyConstruct;
  tloDestructFunction destruct;
} tloType;

bool tloTypeIsValid(const tloType *type);

int tloIntTypeCopyConstruct(void *obj, const void *data);
void tloIntTypeDestruct(void *obj);
extern const tloType tloIntType;

typedef void *(*tloMallocFunction)(size_t byteCount);
typedef void (*tloFreeFunction)(void *bytes);

typedef struct tloAllocator {
  tloMallocFunction malloc;
  tloFreeFunction free;
} tloAllocator;

bool tloAllocatorIsValid(const tloAllocator *allocator);

extern const tloAllocator tloCStdLibAllocator;

extern unsigned long tloCountingAllocatorMallocCount;
extern unsigned long tloCountingAllocatorFreeCount;
void *tloCountingAllocatorMalloc(size_t byteCount);
void tloCountingAllocatorFree(void *bytes);
extern const tloAllocator tloCountingAllocator;

#endif // TLO_UTIL_H
