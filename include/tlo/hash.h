#ifndef TLO_HASH_H
#define TLO_HASH_H

#include <stddef.h>

typedef size_t (*TloHashFunction)(const void *data, size_t length);

/*
 * Hashing algorithms from
 * http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */

size_t tloRotatingHash(const void *data, size_t length);

// Bernstein hash
size_t tloDJBHash(const void *data, size_t length);

// Modified Bernstein hash
size_t tloMDJBHash(const void *data, size_t length);

// Shift-Add-XOR hash
size_t tloSAXHash(const void *data, size_t length);

// Fowler-Noll/Vo (FNV-1) hash
size_t tloFNV1Hash(const void *data, size_t length);

// One-at-a-Time hash
size_t tloOAATHash(const void *data, size_t length);

size_t tloELFHash(const void *data, size_t length);

#endif  // TLO_HASH_H
