#ifndef TLO_SET_H
#define TLO_SET_H

#include "tlo/util.h"

typedef struct TloSet TloSet;

typedef struct TloSetVTable {
  // private

  // all of the following must be implemented
  const char *type;
  void (*destruct)(TloSet *set);
  size_t (*size)(const TloSet *set);
  bool (*isEmpty)(const TloSet *set);
  const void *(*find)(const TloSet *set, const void *key);
  TloError (*insert)(TloSet *set, const void *key);
  TloError (*moveInsert)(TloSet *set, void *key);
  bool (*remove)(TloSet *set, const void *key);
} TloSetVTable;

struct TloSet {
  // private
  const TloSetVTable *vTable;
  const TloType *keyType;
  const TloAllocator *allocator;
};

void tloSetConstruct(TloSet *set, const TloSetVTable *vTable,
                     const TloType *keyType, const TloAllocator *allocator);

/*
 * - tlovSetDestruct then allocator's free
 */
void tloSetDelete(TloSet *set);

const TloType *tloSetValueType(const TloSet *set);
const TloAllocator *tloSetAllocator(const TloSet *set);

const char *tlovSetType(const TloSet *set);

/*
 * - uses key type's destruct if it is not NULL
 */
void tlovSetDestruct(TloSet *set);

size_t tlovSetSize(const TloSet *set);
bool tlovSetIsEmpty(const TloSet *set);
const void *tlovSetFind(const TloSet *set, const void *key);

/*
 * - deep copies key using key type's constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tlovSetInsert(TloSet *set, const void *key);

/*
 * - assumes key points to an object whose memory was allocated by allocator's
 *   malloc
 * - takes ownership of the object
 */
TloError tlovSetMoveInsert(TloSet *set, void *key);

/*
 * - uses key type's destruct if it is not NULL
 */
bool tlovSetRemove(TloSet *set, const void *key);

#endif  // TLO_SET_H
