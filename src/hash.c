#include "tlo/hash.h"
#include <assert.h>
#include <stdint.h>

/*
 * Hashing algorithms from
 * http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */

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

size_t tloELFHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 0;

  for (size_t i = 0; i < length; i++) {
    hash = (hash << 4) + bytes[i];
    size_t highBits = hash & 0xF0000000;

    if (highBits != 0) {
      hash ^= highBits >> 24;
    }

    hash &= ~highBits;
  }

  return hash;
}
