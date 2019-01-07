#ifndef TLO_HASH_H
#define TLO_HASH_H

#include <stddef.h>

typedef size_t (*TloHashFunction)(const void *data, size_t length);

/*
 * Hashing algorithms from
 * http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * http://www.isthe.com/chongo/tech/comp/fnv/
 * https://www.uclibc.org/docs/elf-64-gen.pdf
 * http://www.drdobbs.com/database/hashing-rehashed/184409859
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

// Fowler-Noll/Vo (FNV-1a) hash
size_t tloFNV1aHash(const void *data, size_t length);

// One-at-a-Time hash
size_t tloOAATHash(const void *data, size_t length);

/*
 * The real ELF hash always produces 32-bit hashes, even in ELF-64, because it
 * does bit shifts and masks using values based on 32-bits. This function
 * doesn't behave exactly like the real ELF hash because it will do bit shifts
 * and masks based on the size of size_t. This is the same behavior as in the
 * PJW hash which ELF hash is based on.
 */
size_t tloELFHash(const void *data, size_t length);

#endif  // TLO_HASH_H
