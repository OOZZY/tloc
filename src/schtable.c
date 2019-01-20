#include "tlo/schtable.h"
#include <assert.h>
#include <string.h>
#include "map.h"
#include "set.h"
#include "util.h"

#ifndef NDEBUG
static bool schtableSetIsValid(const TloSet *set) {
  const TloSCHTableSet *htset = (const TloSCHTableSet *)set;
  return setIsValid(set) && (htset->table.size <= htset->table.capacity);
}

static bool schtableMapIsValid(const TloMap *map) {
  const TloSCHTableMap *htmap = (const TloSCHTableMap *)map;
  return mapIsValid(map) && (htmap->table.size <= htmap->table.capacity);
}
#endif

static void deleteSetNode(const TloSet *set, TloSCHTNode *node) {
  tloTypeDestruct(set->keyType, node->data);
  set->allocator->free(node->data);
  set->allocator->free(node);
}

static void deleteMapNode(const TloMap *map, TloSCHTNode *node) {
  tloTypeDestruct(map->valueType, node->data + map->keyType->size);
  tloTypeDestruct(map->keyType, node->data);
  map->allocator->free(node->data);
  map->allocator->free(node);
}

static void schtableSetDestruct(TloSet *set) {
  if (!set) {
    return;
  }

  assert(schtableSetIsValid(set));

  TloSCHTableSet *htset = (TloSCHTableSet *)set;
  if (!htset->table.array) {
    return;
  }

  for (size_t i = 0; i < htset->table.capacity; ++i) {
    TloSCHTNode *node = htset->table.array[i];

    while (node) {
      TloSCHTNode *next = node->next;
      deleteSetNode(set, node);
      node = next;
    }
  }

  htset->set.allocator->free(htset->table.array);
  htset->table.array = NULL;
}

static void schtableMapDestruct(TloMap *map) {
  if (!map) {
    return;
  }

  assert(schtableMapIsValid(map));

  TloSCHTableMap *htmap = (TloSCHTableMap *)map;
  if (!htmap->table.array) {
    return;
  }

  for (size_t i = 0; i < htmap->table.capacity; ++i) {
    TloSCHTNode *node = htmap->table.array[i];

    while (node) {
      TloSCHTNode *next = node->next;
      deleteMapNode(map, node);
      node = next;
    }
  }

  htmap->map.allocator->free(htmap->table.array);
  htmap->table.array = NULL;
}

static size_t schtableSetSize(const TloSet *set) {
  assert(schtableSetIsValid(set));

  const TloSCHTableSet *htset = (const TloSCHTableSet *)set;
  return htset->table.size;
}

static size_t schtableMapSize(const TloMap *map) {
  assert(schtableMapIsValid(map));

  const TloSCHTableMap *htmap = (const TloSCHTableMap *)map;
  return htmap->table.size;
}

static bool schtableSetIsEmpty(const TloSet *set) {
  assert(schtableSetIsValid(set));

  const TloSCHTableSet *htset = (const TloSCHTableSet *)set;
  return htset->table.size == 0;
}

static bool schtableMapIsEmpty(const TloMap *map) {
  assert(schtableMapIsValid(map));

  const TloSCHTableMap *htmap = (const TloSCHTableMap *)map;
  return htmap->table.size == 0;
}

typedef struct FindResult {
  size_t hash;
  size_t index;
  TloSCHTNode *prev;
  TloSCHTNode *node;
} FindResult;

static void find(FindResult *result, const TloSCHTable *table,
                 const TloType *keyType, const void *key) {
  result->hash = tloTypeHash(keyType, key);
  result->index = result->hash % table->capacity;
  result->prev = NULL;

  for (TloSCHTNode *node = table->array[result->index]; node;
       node = node->next) {
    if (tloTypeEquals(keyType, node->data, key)) {
      result->node = node;
      return;
    }

    result->prev = node;
  }

  result->prev = NULL;
  result->node = NULL;
}

static const void *schtableSetFind(const TloSet *set, const void *key) {
  assert(schtableSetIsValid(set));
  assert(key);

  FindResult result;
  const TloSCHTableSet *htset = (const TloSCHTableSet *)set;

  find(&result, &htset->table, set->keyType, key);
  if (!result.node) {
    return NULL;
  }

  return result.node->data;
}

static const void *schtableMapFind(const TloMap *map, const void *key) {
  assert(schtableMapIsValid(map));
  assert(key);

  FindResult result;
  const TloSCHTableMap *htmap = (const TloSCHTableMap *)map;

  find(&result, &htmap->table, map->keyType, key);
  if (!result.node) {
    return NULL;
  }

  return result.node->data + map->keyType->size;
}

static void *schtableMapFindMutable(TloMap *map, const void *key) {
  assert(schtableMapIsValid(map));
  assert(key);

  FindResult result;
  TloSCHTableMap *htmap = (TloSCHTableMap *)map;

  find(&result, &htmap->table, map->keyType, key);
  if (!result.node) {
    return NULL;
  }

  return result.node->data + map->keyType->size;
}

static TloSCHTNode *makeSetNodeWithCopiedData(const TloSet *set,
                                              const void *key) {
  TloSCHTNode *node = set->allocator->malloc(sizeof(*node));
  if (!node) {
    goto error0;
  }

  node->data = set->allocator->malloc(set->keyType->size);
  if (!node->data) {
    goto error1;
  }

  if (tloTypeConstructCopy(set->keyType, node->data, key) != TLO_SUCCESS) {
    goto error2;
  }

  node->next = NULL;
  return node;

error2:
  set->allocator->free(node->data);
error1:
  set->allocator->free(node);
error0:
  return NULL;
}

static TloSCHTNode *makeMapNodeWithCopiedData(const TloMap *map,
                                              const void *key,
                                              const void *value) {
  TloSCHTNode *node = map->allocator->malloc(sizeof(*node));
  if (!node) {
    goto error0;
  }

  node->data =
      map->allocator->malloc(map->keyType->size + map->valueType->size);
  if (!node->data) {
    goto error1;
  }

  if (tloTypeConstructCopy(map->keyType, node->data, key) != TLO_SUCCESS) {
    goto error2;
  }

  if (tloTypeConstructCopy(map->valueType, node->data + map->keyType->size,
                           value) != TLO_SUCCESS) {
    goto error3;
  }

  node->next = NULL;
  return node;

error3:
  tloTypeDestruct(map->keyType, node->data);
error2:
  map->allocator->free(node->data);
error1:
  map->allocator->free(node);
error0:
  return NULL;
}

static TloSCHTNode *makeSetNodeWithMovedData(const TloAllocator *allocator,
                                             void *key) {
  TloSCHTNode *node = allocator->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->data = key;
  node->next = NULL;
  return node;
}

static TloSCHTNode *makeMapNodeWithMovedData(const TloMap *map, void *key,
                                             void *value) {
  TloSCHTNode *node = map->allocator->malloc(sizeof(*node));
  if (!node) {
    goto error0;
  }

  node->data =
      map->allocator->malloc(map->keyType->size + map->valueType->size);
  if (!node->data) {
    goto error1;
  }

  memcpy(node->data, key, map->keyType->size);
  map->allocator->free(key);

  memcpy(node->data + map->keyType->size, value, map->valueType->size);
  map->allocator->free(value);

  node->next = NULL;
  return node;

error1:
  map->allocator->free(node);
error0:
  return NULL;
}

enum { STARTING_CAPACITY = 1 };

static TloError allocateArrayIfNeeded(TloSCHTable *table,
                                      const TloAllocator *allocator) {
  if (!table->array) {
    table->array = allocator->malloc(STARTING_CAPACITY * sizeof(*table->array));
    if (!table->array) {
      return TLO_ERROR;
    }

    for (int i = 0; i < STARTING_CAPACITY; ++i) {
      table->array[i] = NULL;
    }

    table->capacity = STARTING_CAPACITY;
  }
  return TLO_SUCCESS;
}

static TloError insertNode(TloSCHTable *table, const TloType *keyType,
                           const TloAllocator *allocator, size_t *hash,
                           TloSCHTNode *node);

static void insertAllNodesOfOther(TloSCHTable *table, const TloSCHTable *other,
                                  const TloType *keyType,
                                  const TloAllocator *allocator) {
  for (size_t i = 0; i < other->capacity; ++i) {
    for (TloSCHTNode *node = other->array[i]; node; node = node->next) {
      insertNode(table, keyType, allocator, NULL, node);
    }
  }
}

static TloError expandArrayIfNeeded(TloSCHTable *table, const TloType *keyType,
                                    const TloAllocator *allocator) {
  if (table->size == table->capacity) {
    TloSCHTable newTable;

    newTable.size = 0;
    newTable.capacity = table->capacity * 2;
    newTable.array =
        allocator->malloc(newTable.capacity * sizeof(*newTable.array));
    if (!newTable.array) {
      return TLO_ERROR;
    }

    insertAllNodesOfOther(&newTable, table, keyType, allocator);
    allocator->free(table->array);
    table->array = newTable.array;
    table->capacity = newTable.capacity;
  }

  return TLO_SUCCESS;
}

static TloError insertNode(TloSCHTable *table, const TloType *keyType,
                           const TloAllocator *allocator, size_t *hash,
                           TloSCHTNode *node) {
  if (allocateArrayIfNeeded(table, allocator) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  if (expandArrayIfNeeded(table, keyType, allocator) != TLO_SUCCESS) {
    return TLO_ERROR;
  }

  size_t index;

  if (hash) {
    index = *hash % table->capacity;
  } else {
    index = tloTypeHash(keyType, node->data) % table->capacity;
  }

  if (table->array[index]) {
    node->next = table->array[index];
  }

  table->array[index] = node;
  ++table->size;
  return TLO_SUCCESS;
}

static TloError schtableSetInsert(TloSet *set, const void *key) {
  assert(schtableSetIsValid(set));
  assert(key);

  FindResult result;
  TloSCHTableSet *htset = (TloSCHTableSet *)set;

  find(&result, &htset->table, set->keyType, key);
  if (result.node) {
    return TLO_DUPLICATE;
  }

  TloSCHTNode *newNode = makeSetNodeWithCopiedData(set, key);
  if (!newNode) {
    return TLO_ERROR;
  }

  if (insertNode(&htset->table, set->keyType, set->allocator, &result.hash,
                 newNode) != TLO_SUCCESS) {
    deleteSetNode(set, newNode);
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static TloError schtableMapInsert(TloMap *map, const void *key,
                                  const void *value) {
  assert(schtableMapIsValid(map));
  assert(key);
  assert(value);

  FindResult result;
  TloSCHTableMap *htmap = (TloSCHTableMap *)map;

  find(&result, &htmap->table, map->keyType, key);
  if (result.node) {
    return TLO_DUPLICATE;
  }

  TloSCHTNode *newNode = makeMapNodeWithCopiedData(map, key, value);
  if (!newNode) {
    return TLO_ERROR;
  }

  if (insertNode(&htmap->table, map->keyType, map->allocator, &result.hash,
                 newNode) != TLO_SUCCESS) {
    deleteMapNode(map, newNode);
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static TloError schtableSetMoveInsert(TloSet *set, void *key) {
  assert(schtableSetIsValid(set));
  assert(key);

  FindResult result;
  TloSCHTableSet *htset = (TloSCHTableSet *)set;

  find(&result, &htset->table, set->keyType, key);
  if (result.node) {
    return TLO_DUPLICATE;
  }

  TloSCHTNode *newNode = makeSetNodeWithMovedData(set->allocator, key);
  if (!newNode) {
    return TLO_ERROR;
  }

  if (insertNode(&htset->table, set->keyType, set->allocator, &result.hash,
                 newNode) != TLO_SUCCESS) {
    deleteSetNode(set, newNode);
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static TloError schtableMapMoveInsert(TloMap *map, void *key, void *value) {
  assert(schtableMapIsValid(map));
  assert(key);
  assert(value);

  FindResult result;
  TloSCHTableMap *htmap = (TloSCHTableMap *)map;

  find(&result, &htmap->table, map->keyType, key);
  if (result.node) {
    return TLO_DUPLICATE;
  }

  TloSCHTNode *newNode = makeMapNodeWithMovedData(map, key, value);
  if (!newNode) {
    return TLO_ERROR;
  }

  if (insertNode(&htmap->table, map->keyType, map->allocator, &result.hash,
                 newNode) != TLO_SUCCESS) {
    deleteMapNode(map, newNode);
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void shrinkArrayIfNeeded(TloSCHTable *table, const TloType *keyType,
                                const TloAllocator *allocator) {
  if (table->size <= table->capacity / 4 && table->size) {
    TloSCHTable newTable;

    newTable.size = 0;
    newTable.capacity = table->capacity / 2;
    newTable.array =
        allocator->malloc(newTable.capacity * sizeof(*newTable.array));
    if (!newTable.array) {
      return;
    }

    insertAllNodesOfOther(&newTable, table, keyType, allocator);
    allocator->free(table->array);
    table->array = newTable.array;
    table->capacity = newTable.capacity;
  }
}

static bool schtableSetRemove(TloSet *set, const void *key) {
  assert(schtableSetIsValid(set));
  assert(key);

  FindResult result;
  TloSCHTableSet *htset = (TloSCHTableSet *)set;

  find(&result, &htset->table, set->keyType, key);
  if (!result.node) {
    return false;
  }

  if (result.prev) {
    result.prev->next = result.node->next;
  } else {
    htset->table.array[result.index] = NULL;
  }

  deleteSetNode(set, result.node);
  htset->table.size--;
  shrinkArrayIfNeeded(&htset->table, set->keyType, set->allocator);
  return true;
}

static bool schtableMapRemove(TloMap *map, const void *key) {
  assert(schtableMapIsValid(map));
  assert(key);

  FindResult result;
  TloSCHTableMap *htmap = (TloSCHTableMap *)map;

  find(&result, &htmap->table, map->keyType, key);
  if (!result.node) {
    return false;
  }

  if (result.prev) {
    result.prev->next = result.node->next;
  } else {
    htmap->table.array[result.index] = NULL;
  }

  deleteMapNode(map, result.node);
  htmap->table.size--;
  shrinkArrayIfNeeded(&htmap->table, map->keyType, map->allocator);
  return true;
}

static const TloSetVTable setVTable = {.destruct = schtableSetDestruct,
                                       .size = schtableSetSize,
                                       .isEmpty = schtableSetIsEmpty,
                                       .find = schtableSetFind,
                                       .insert = schtableSetInsert,
                                       .moveInsert = schtableSetMoveInsert,
                                       .remove = schtableSetRemove};

static const TloMapVTable mapVTable = {.destruct = schtableMapDestruct,
                                       .size = schtableMapSize,
                                       .isEmpty = schtableMapIsEmpty,
                                       .find = schtableMapFind,
                                       .findMutable = schtableMapFindMutable,
                                       .insert = schtableMapInsert,
                                       .moveInsert = schtableMapMoveInsert,
                                       .remove = schtableMapRemove};

static void schtableConstruct(TloSCHTable *table) {
  table->array = NULL;
  table->size = 0;
  table->capacity = 0;
}

void tloSCHTableSetConstruct(TloSCHTableSet *htset, const TloType *keyType,
                             const TloAllocator *allocator) {
  assert(htset);
  assert(typeIsValid(keyType));
  assert(allocator == NULL || allocatorIsValid(allocator));

  tloSetConstruct(&htset->set, &setVTable, keyType, allocator);
  schtableConstruct(&htset->table);
}

void tloSCHTableMapConstruct(TloSCHTableMap *htmap, const TloType *keyType,
                             const TloType *valueType,
                             const TloAllocator *allocator) {
  assert(htmap);
  assert(typeIsValid(keyType));
  assert(typeIsValid(valueType));
  assert(allocator == NULL || allocatorIsValid(allocator));

  tloMapConstruct(&htmap->map, &mapVTable, keyType, valueType, allocator);
  schtableConstruct(&htmap->table);
}
