#ifndef TLO_MAP_H
#define TLO_MAP_H

#include "tlo/util.h"

typedef struct TloMap TloMap;

typedef enum TloInsertMethod { TLO_COPY, TLO_MOVE } TloInsertMethod;

typedef struct TloMapVTable {
  // private

  // all of the following must be implemented
  const char *type;
  void (*destruct)(TloMap *map);
  size_t (*size)(const TloMap *map);
  bool (*isEmpty)(const TloMap *map);
  const void *(*find)(const TloMap *map, const void *key);
  void *(*findMutable)(TloMap *map, const void *key);
  TloError (*insert)(TloMap *map, TloInsertMethod keyInsertMethod, void *key,
                     TloInsertMethod valueInsertMethod, void *value);
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
 * - if insert method is TLO_COPY:
 *   - deep copies using key or value type's constructCopy if it is not null
 *   - uses memcpy if type's constructCopy is null
 * - if insert method is TLO_MOVE:
 *   - assumes key or value points to object whose memory was allocated by
 *     allocator's malloc
 *   - takes ownership of the objects
 */
TloError tlovMapInsert(TloMap *map, TloInsertMethod keyInsertMethod, void *key,
                       TloInsertMethod valueInsertMethod, void *value);

/*
 * - uses key type's destruct if it is not NULL
 * - uses value type's destruct if it is not NULL
 */
bool tlovMapRemove(TloMap *map, const void *key);

#endif  // TLO_MAP_H
