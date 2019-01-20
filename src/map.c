#include "map.h"
#include <assert.h>
#include "util.h"

static bool mapVTableIsValid(const TloMapVTable *vTable) {
  return vTable && vTable->type && vTable->destruct && vTable->size &&
         vTable->isEmpty && vTable->find && vTable->insert &&
         vTable->moveInsert && vTable->remove;
}

void tloMapConstruct(TloMap *map, const TloMapVTable *vTable,
                     const TloType *keyType, const TloType *valueType,
                     const TloAllocator *allocator) {
  assert(map);
  assert(mapVTableIsValid(vTable));
  assert(typeIsValid(keyType));
  assert(typeIsValid(valueType));

  if (!allocator) {
    allocator = &tloCStdLibAllocator;
  }

  assert(allocatorIsValid(allocator));

  map->vTable = vTable;
  map->keyType = keyType;
  map->valueType = valueType;
  map->allocator = allocator;
}

void tloMapDelete(TloMap *map) {
  if (!map) {
    return;
  }

  assert(mapIsValid(map));

  tlovMapDestruct(map);
  map->allocator->free(map);
}

const TloType *tloMapKeyType(const TloMap *map) {
  assert(mapIsValid(map));

  return map->keyType;
}

const TloType *tloMapValueType(const TloMap *map) {
  assert(mapIsValid(map));

  return map->valueType;
}

const TloAllocator *tloMapAllocator(const TloMap *map) {
  assert(mapIsValid(map));

  return map->allocator;
}

const char *tlovMapType(const TloMap *map) {
  assert(mapIsValid(map));

  return map->vTable->type;
}

void tlovMapDestruct(TloMap *map) {
  assert(mapIsValid(map));

  map->vTable->destruct(map);
}

size_t tlovMapSize(const TloMap *map) {
  assert(mapIsValid(map));

  return map->vTable->size(map);
}

bool tlovMapIsEmpty(const TloMap *map) {
  assert(mapIsValid(map));

  return map->vTable->isEmpty(map);
}

const void *tlovMapFind(const TloMap *map, const void *key) {
  assert(mapIsValid(map));

  return map->vTable->find(map, key);
}

void *tlovMapFindMutable(TloMap *map, const void *key) {
  assert(mapIsValid(map));

  return map->vTable->findMutable(map, key);
}

TloError tlovMapInsert(TloMap *map, const void *key, const void *value) {
  assert(mapIsValid(map));

  return map->vTable->insert(map, key, value);
}

TloError tlovMapMoveInsert(TloMap *map, void *key, void *value) {
  assert(mapIsValid(map));

  return map->vTable->moveInsert(map, key, value);
}

bool tlovMapRemove(TloMap *map, const void *key) {
  assert(mapIsValid(map));

  return map->vTable->remove(map, key);
}

bool mapIsValid(const TloMap *map) {
  return map && mapVTableIsValid(map->vTable) && typeIsValid(map->keyType) &&
         typeIsValid(map->valueType) && allocatorIsValid(map->allocator);
}
