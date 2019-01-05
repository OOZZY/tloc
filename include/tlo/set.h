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
  const void *(*find)(const TloSet *set, const void *data);
  TloError (*insert)(TloSet *set, const void *data);
  TloError (*moveInsert)(TloSet *set, void *data);
  void (*remove)(TloSet *set, const void *data);
} TloSetVTable;

struct TloSet {
  // private
  const TloSetVTable *vTable;
  const TloType *valueType;
  const TloAllocator *allocator;
};

void tloSetConstruct(TloSet *set, const TloSetVTable *vTable,
                     const TloType *valueType, const TloAllocator *allocator);

/*
 * - tlovSetDestruct then allocator's free
 */
void tloSetDelete(TloSet *set);

const TloType *tloSetValueType(const TloSet *set);
const TloAllocator *tloSetAllocator(const TloSet *set);

const char *tlovSetType(const TloSet *set);

/*
 * - uses value type's destruct if it is not NULL
 */
void tlovSetDestruct(TloSet *set);

size_t tlovSetSize(const TloSet *set);
bool tlovSetIsEmpty(const TloSet *set);
const void *tlovSetFind(const TloSet *set, const void *data);

/*
 * - deep copies data using value type's constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tlovSetInsert(TloSet *set, const void *data);

/*
 * - assumes data points to an object whose memory was allocated by allocator's
 *   malloc
 * - takes ownership of the object
 */
TloError tlovSetMoveInsert(TloSet *set, void *data);

/*
 * - uses value type's destruct if it is not NULL
 */
void tlovSetRemove(TloSet *set, const void *data);

#endif  // TLO_SET_H
