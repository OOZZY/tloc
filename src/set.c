#include "set.h"
#include <assert.h>
#include "util.h"

static bool setVTableIsValid(const TloSetVTable *vTable) {
  return vTable && vTable->type && vTable->destruct && vTable->size &&
         vTable->isEmpty && vTable->find && vTable->insert &&
         vTable->moveInsert && vTable->remove;
}

void tloSetConstruct(TloSet *set, const TloSetVTable *vTable,
                     const TloType *keyType, const TloAllocator *allocator) {
  assert(set);
  assert(setVTableIsValid(vTable));
  assert(typeIsValid(keyType));

  if (!allocator) {
    allocator = &tloCStdLibAllocator;
  }

  assert(allocatorIsValid(allocator));

  set->vTable = vTable;
  set->keyType = keyType;
  set->allocator = allocator;
}

void tloSetDelete(TloSet *set) {
  if (!set) {
    return;
  }

  assert(setIsValid(set));

  tlovSetDestruct(set);
  set->allocator->free(set);
}

const TloType *tloSetValueType(const TloSet *set) {
  assert(setIsValid(set));

  return set->keyType;
}

const TloAllocator *tloSetAllocator(const TloSet *set) {
  assert(setIsValid(set));

  return set->allocator;
}

const char *tlovSetType(const TloSet *set) {
  assert(setIsValid(set));

  return set->vTable->type;
}

void tlovSetDestruct(TloSet *set) {
  assert(setIsValid(set));

  set->vTable->destruct(set);
}

size_t tlovSetSize(const TloSet *set) {
  assert(setIsValid(set));

  return set->vTable->size(set);
}

bool tlovSetIsEmpty(const TloSet *set) {
  assert(setIsValid(set));

  return set->vTable->isEmpty(set);
}

const void *tlovSetFind(const TloSet *set, const void *key) {
  assert(setIsValid(set));

  return set->vTable->find(set, key);
}

TloError tlovSetInsert(TloSet *set, const void *key) {
  assert(setIsValid(set));

  return set->vTable->insert(set, key);
}

TloError tlovSetMoveInsert(TloSet *set, void *key) {
  assert(setIsValid(set));

  return set->vTable->moveInsert(set, key);
}

bool tlovSetRemove(TloSet *set, const void *key) {
  assert(setIsValid(set));

  return set->vTable->remove(set, key);
}

bool setIsValid(const TloSet *set) {
  return set && setVTableIsValid(set->vTable) && typeIsValid(set->keyType) &&
         allocatorIsValid(set->allocator);
}
