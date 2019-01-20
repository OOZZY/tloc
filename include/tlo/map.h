#ifndef TLO_MAP_H
#define TLO_MAP_H

#include "tlo/util.h"

typedef struct TloMap TloMap;

typedef struct TloMapVTable {
  // private

  // all of the following must be implemented
  const char *type;
  void (*destruct)(TloMap *map);
  size_t (*size)(const TloMap *map);
  bool (*isEmpty)(const TloMap *map);
  const void *(*find)(const TloMap *map, const void *key);
  void *(*findMutable)(TloMap *map, const void *key);
  TloError (*insert)(TloMap *map, const void *key, const void *value);
  TloError (*moveInsert)(TloMap *map, void *key, void *value);
  bool (*remove)(TloMap *map, const void *key);
} TloMapVTable;

struct TloMap {
  // private
  const TloMapVTable *vTable;
  const TloType *keyType;
  const TloType *valueType;
  const TloAllocator *allocator;
};

void tloMapConstruct(TloMap *map, const TloMapVTable *vTable,
                     const TloType *keyType, const TloType *valueType,
                     const TloAllocator *allocator);

/*
 * - tlovMapDestruct then allocator's free
 */
void tloMapDelete(TloMap *map);

const TloType *tloMapKeyType(const TloMap *map);
const TloType *tloMapValueType(const TloMap *map);
const TloAllocator *tloMapAllocator(const TloMap *map);

const char *tlovMapType(const TloMap *map);

/*
 * - uses key type's destruct if it is not NULL
 * - uses value type's destruct if it is not NULL
 */
void tlovMapDestruct(TloMap *map);

size_t tlovMapSize(const TloMap *map);
bool tlovMapIsEmpty(const TloMap *map);
const void *tlovMapFind(const TloMap *map, const void *key);
void *tlovMapFindMutable(TloMap *map, const void *key);

/*
 * - deep copies key using key type's constructCopy if it is not null
 * - deep copies value using value type's constructCopy if it is not null
 * - uses memcpy if type's constructCopy is null
 */
TloError tlovMapInsert(TloMap *map, const void *key, const void *value);

/*
 * - assumes key and value point to objects whose memory was allocated by
 *   allocator's malloc
 * - takes ownership of the objects
 */
TloError tlovMapMoveInsert(TloMap *map, void *key, void *value);

/*
 * - uses key type's destruct if it is not NULL
 * - uses value type's destruct if it is not NULL
 */
bool tlovMapRemove(TloMap *map, const void *key);

#endif  // TLO_MAP_H
