#include "tlo/hash.h"
#include <assert.h>

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

size_t tloFNVHash(const void *data, size_t length) {
  assert(data);

  const unsigned char *bytes = data;
  size_t hash = 2166136261;

  for (size_t i = 0; i < length; i++) {
    hash = (hash * 16777619) ^ bytes[i];
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
  size_t hash = 0, highBits;

  for (size_t i = 0; i < length; i++) {
    hash = (hash << 4) + bytes[i];
    highBits = hash & 0xF0000000;

    if (highBits != 0) {
      hash ^= highBits >> 24;
    }

    hash &= ~highBits;
  }

  return hash;
}
