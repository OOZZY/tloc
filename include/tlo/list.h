#ifndef TLO_LIST_H
#define TLO_LIST_H

#include "tlo/util.h"

typedef struct TloList TloList;

typedef struct TloListVTable {
  // private

  // all of the following must be implemented
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

  // all of the following are optional
  size_t (*capacity)(const TloList *list);
  const void *(*element)(const TloList *list, size_t index);
  void *(*mutableElement)(TloList *list, size_t index);
  TloError (*pushFront)(TloList *list, const void *data);
  TloError (*moveFront)(TloList *list, void *data);
  void (*popFront)(TloList *list);
  void (*popBack)(TloList *list);
  void (*unorderedRemove)(TloList *list, size_t index);
} TloListVTable;

bool tloListVTableIsValid(const TloListVTable *vTable);

typedef enum TloListOptionalFunction {
  TLO_LIST_CAPACITY = 1U,
  TLO_LIST_ELEMENT = 1U << 1,
  TLO_LIST_PUSH_FRONT = 1U << 2,
  TLO_LIST_POP_FRONT = 1U << 3,
  TLO_LIST_POP_BACK = 1U << 4,
  TLO_LIST_UNORDERED_REMOVE = 1U << 5
} TloListOptionalFunction;

struct TloList {
  // private
  const TloListVTable *vTable;
  const TloType *valueType;
  const TloAllocator *allocator;
};

bool tloListIsValid(const TloList *list);
void tloListConstruct(TloList *list, const TloListVTable *vTable,
                      const TloType *valueType, const TloAllocator *allocator);

/*
 * - tlovListDestruct then allocator's free
 */
void tloListDelete(TloList *list);

const TloType *tloListValueType(const TloList *list);
const TloAllocator *tloListAllocator(const TloList *list);
bool tloListHasFunctions(const TloList *list, unsigned char functions);

const char *tlovListType(const TloList *list);
bool tlovListIsValid(const TloList *list);

/*
 * - uses value type's destruct if it is not NULL
 */
void tlovListDestruct(TloList *list);

size_t tlovListSize(const TloList *list);
bool tlovListIsEmpty(const TloList *list);
const void *tlovListFront(const TloList *list);
void *tlovListMutableFront(TloList *list);
const void *tlovListBack(const TloList *list);
void *tlovListMutableBack(TloList *list);

/*
 * - deep copies data using value type's constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tlovListPushBack(TloList *list, const void *data);

/*
 * - assumes data points to an object whose memory was allocated by allocator's
 *   malloc
 * - takes ownership of the object
 */
TloError tlovListMoveBack(TloList *list, void *data);

// assumes tloListHasFunctions(list, TLO_LIST_CAPACITY)
size_t tlovListCapacity(const TloList *list);

// assumes tloListHasFunctions(list, TLO_LIST_ELEMENT)
const void *tlovListElement(const TloList *list, size_t index);

// assumes tloListHasFunctions(list, TLO_LIST_ELEMENT)
void *tlovListMutableElement(TloList *list, size_t index);

/*
 * - assumes tloListHasFunctions(list, TLO_LIST_PUSH_FRONT)
 * - deep copies data using value type's constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tlovListPushFront(TloList *list, const void *data);

/*
 * - assumes tloListHasFunctions(list, TLO_LIST_PUSH_FRONT)
 * - assumes data points to an object whose memory was allocated by allocator's
 *   malloc
 * - takes ownership of the object
 */
TloError tlovListMoveFront(TloList *list, void *data);

/*
 * - assumes tloListHasFunctions(list, TLO_LIST_POP_FRONT)
 * - uses value type's destruct if it is not NULL
 */
void tlovListPopFront(TloList *list);

/*
 * - assumes tloListHasFunctions(list, TLO_LIST_POP_BACK)
 * - uses value type's destruct if it is not NULL
 */
void tlovListPopBack(TloList *list);

/*
 * - assumes tloListHasFunctions(list, TLO_LIST_UNORDERED_REMOVE)
 * - uses value type's destruct if it is not NULL
 */
void tlovListUnorderedRemove(TloList *list, size_t index);

#endif  // TLO_LIST_H
