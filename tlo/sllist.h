#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/util.h"

/*
 * - singly-linked list
 */
typedef struct TloSLLNode {
  // private
  void *bytes;
  struct TloSLLNode *next;
} TloSLLNode;

typedef struct TloSLList {
  // private
  const TloType *valueType;
  const TloAllocatorType *allocatorType;
  TloSLLNode *head;
  TloSLLNode *tail;
  size_t size;
} TloSLList;

bool tloSLListIsValid(const TloSLList *list);
TloError tloSLListConstruct(TloSLList *list, const TloType *valueType,
                            const TloAllocatorType *allocatorType);

/*
 * - deep copies data using other->valueType->constructCopy
 */
TloError tloSLListConstructCopy(TloSLList *list, const TloSLList *other);

/*
 * - uses list->valueType->destruct if it is not NULL
 */
void tloSLListDestruct(TloSLList *list);

/*
 * - allocatorType->malloc then tloSLListConstruct
 */
TloSLList *tloSLListMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType);

/*
 * - other->allocatorType->malloc then tloSLListConstructCopy
 */
TloSLList *tloSLListMakeCopy(const TloSLList *other);

/*
 * - tloSLListDestruct then list->allocatorType->free
 */
void tloSLListDelete(TloSLList *list);

TloError tloSLListCopy(TloSLList *list, const TloSLList *other);
const TloType *tloSLListValueType(const TloSLList *list);
const TloAllocatorType *tloSLListAllocatorType(const TloSLList *list);
size_t tloSLListSize(const TloSLList *list);
bool tloSLListIsEmpty(const TloSLList *list);
const void *tloSLListFront(const TloSLList *list);
void *tloSLListMutableFront(TloSLList *list);
const void *tloSLListBack(const TloSLList *list);
void *tloSLListMutableBack(TloSLList *list);

/*
 * - deep copies data using list->valueType->constructCopy
 */
TloError tloSLListPushFront(TloSLList *list, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
TloError tloSLListMoveFront(TloSLList *list, void *data);

/*
 * - uses list->valueType->destruct if it is not NULL
 */
void tloSLListPopFront(TloSLList *list);

/*
 * - deep copies data using list->valueType->constructCopy
 */
TloError tloSLListPushBack(TloSLList *list, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
TloError tloSLListMoveBack(TloSLList *list, void *data);

bool tloSLLNodeIsValid(const TloSLLNode *node);
const TloSLLNode *tloSLLNodeHead(const TloSLList *list);
TloSLLNode *tloSLLNodeMutableHead(TloSLList *list);
const void *tloSLLNodeElement(const TloSLLNode *node);
void *tloSLLNodeMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
