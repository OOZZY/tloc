#include "tlo/hash.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

size_t tloRotatingHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = (hash << 4) ^ (hash >> 28) ^ bytes[i];
  }

  return hash;
}

size_t tloDJBHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = 33 * hash + bytes[i];
  }

  return hash;
}

size_t tloMDJBHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = 33 * hash ^ bytes[i];
  }

  return hash;
}

size_t tloSAXHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash ^= (hash << 5) + (hash >> 2) + bytes[i];
  }

  return hash;
}

#if SIZE_MAX == 0xFFFFFFFF
#define OFFSET_BASIS 2166136261UL
#define FNV_PRIME 16777619UL
#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF
#define OFFSET_BASIS 14695981039346656037ULL
#define FNV_PRIME 1099511628211ULL
#else
#error "tlo/hash: FNV algorithms implemented for only 32-bit and 64-bit size_t"
#endif

size_t tloFNV1Hash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = OFFSET_BASIS;

  for (size_t i = 0; i < length; i++) {
    hash = (hash * FNV_PRIME) ^ bytes[i];
  }

  return hash;
}

size_t tloFNV1aHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = OFFSET_BASIS;

  for (size_t i = 0; i < length; i++) {
    hash = (hash ^ bytes[i]) * FNV_PRIME;
  }

  return hash;
}

size_t tloOAATHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash += bytes[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

#define SIZE_T_BITS (sizeof(size_t) * CHAR_BIT)
#define ONE_EIGHTH (SIZE_T_BITS / 8)
#define THREE_QUARTERS (SIZE_T_BITS * 3 / 4)
#define HIGH_BITS_MASK (~(SIZE_MAX >> ONE_EIGHTH))

size_t tloELFHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = (hash << ONE_EIGHTH) + bytes[i];
    size_t highBits = hash & HIGH_BITS_MASK;

    if (highBits != 0) {
      hash ^= highBits >> THREE_QUARTERS;
    }

    hash &= ~highBits;
  }

  return hash;
}

size_t tloPJWHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = (hash << ONE_EIGHTH) + bytes[i];
    size_t highBits = hash & HIGH_BITS_MASK;

    if (highBits != 0) {
      hash ^= highBits >> THREE_QUARTERS;
      hash &= ~highBits;
    }
  }

  return hash;
}
