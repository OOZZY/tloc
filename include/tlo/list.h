#ifndef TLO_LIST_H
#define TLO_LIST_H

#include "tlo/util.h"

typedef struct TloList TloList;

typedef struct TloListVTable {
  // private
  const char *type;
  bool (*isValid)(const TloList *list);
  void (*destruct)(TloList *list);
  size_t (*size)(const TloList *list);
  bool (*isEmpty)(const TloList *list);
  const void *(*front)(const TloList *list);
  void *(*mutableFront)(TloList *list);
  const void *(*back)(const TloList *list);
  void *(*mutableBack)(TloList *list);
  TloError (*pushBack)(TloList *list, const void *data);
  TloError (*moveBack)(TloList *list, void *data);
} TloListVTable;

bool tloListVTableIsValid(const TloListVTable *vTable);

struct TloList {
  // private
  const TloListVTable *vTable;
  const TloType *valueType;
  const TloAllocatorType *allocatorType;
};

bool tloListIsValid(const TloList *list);
void tloListConstruct(TloList *list, const TloListVTable *vTable,
                      const TloType *valueType,
                      const TloAllocatorType *allocatorType);

/*
 * - tlovListDestruct then list->allocatorType->free
 */
void tloListDelete(TloList *list);

const TloType *tloListValueType(const TloList *list);
const TloAllocatorType *tloListAllocatorType(const TloList *list);

const char *tlovListType(const TloList *list);
bool tlovListIsValid(const TloList *list);

/*
 * - uses list->valueType->destruct if it is not NULL
 */
void tlovListDestruct(TloList *list);

size_t tlovListSize(const TloList *list);
bool tlovListIsEmpty(const TloList *list);
const void *tlovListFront(const TloList *list);
void *tlovListMutableFront(TloList *list);
const void *tlovListBack(const TloList *list);
void *tlovListMutableBack(TloList *list);

/*
 * - deep copies data using list->valueType->constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tlovListPushBack(TloList *list, const void *data);

/*
 * - assumes data points to an object whose memory was allocated by
 *   allocatorType->malloc
 * - takes ownership of the object
 */
TloError tlovListMoveBack(TloList *list, void *data);

#endif  // TLO_LIST_H
