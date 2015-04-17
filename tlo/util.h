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
 * * both bytes and data should not be NULL. they should point to some valid
 *   space such as the following:
 *   * heap space allocated by some malloc implementation
 *   * stack space
 * * should do a deep copy
 */
typedef int (*tloConstructCopyFunction)(void *bytes, const void *data);

/*
 * * does nothing if bytes is NULL
 * * should not fail
 */
typedef void (*tloDestructFunction)(void *bytes);

typedef struct tloType {
  // public
  size_t sizeOf;
  tloConstructCopyFunction constructCopy;
  tloDestructFunction destruct;
} tloType;

bool tloTypeIsValid(const tloType *type);

int tloIntTypeConstructCopy(void *bytes, const void *data);
void tloIntTypeDestruct(void *bytes);
extern const tloType tloIntType;

typedef void *(*tloMallocFunction)(size_t byteCount);
typedef void (*tloFreeFunction)(void *bytes);

typedef struct tloAllocator {
  // public
  tloMallocFunction malloc;
  tloFreeFunction free;
} tloAllocator;

bool tloAllocatorIsValid(const tloAllocator *allocator);

extern const tloAllocator tloCStdLibAllocator;

extern unsigned long tloCountingAllocatorMallocCount;
extern unsigned long tloCountingAllocatorFreeCount;
extern unsigned long tloCountingAllocatorTotalByteCount;
void *tloCountingAllocatorMalloc(size_t byteCount);
void tloCountingAllocatorFree(void *bytes);
extern const tloAllocator tloCountingAllocator;

#endif  // TLO_UTIL_H
