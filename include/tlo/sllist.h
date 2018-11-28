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

bool tloSLListIsValid(const TloSLList *llist);
TloError tloSLListConstruct(TloSLList *llist, const TloType *valueType,
                            const TloAllocatorType *allocatorType);

/*
 * - uses tloSLListPushBack
 */
TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other);

/*
 * - uses llist->valueType->destruct if it is not NULL
 */
void tloSLListDestruct(TloSLList *llist);

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
 * - tloSLListDestruct then llist->allocatorType->free
 */
void tloSLListDelete(TloSLList *llist);

/*
 * - uses tloSLListConstructCopy and tloSLListDestruct
 */
TloError tloSLListCopy(TloSLList *llist, const TloSLList *other);

const TloType *tloSLListValueType(const TloSLList *llist);
const TloAllocatorType *tloSLListAllocatorType(const TloSLList *llist);
size_t tloSLListSize(const TloSLList *llist);
bool tloSLListIsEmpty(const TloSLList *llist);
const void *tloSLListFront(const TloSLList *llist);
void *tloSLListMutableFront(TloSLList *llist);
const void *tloSLListBack(const TloSLList *llist);
void *tloSLListMutableBack(TloSLList *llist);

/*
 * - deep copies data using llist->valueType->constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tloSLListPushFront(TloSLList *llist, const void *data);

/*
 * - assumes data points to an object whose memory was allocated by
 *   allocatorType->malloc
 * - takes ownership of the object
 */
TloError tloSLListMoveFront(TloSLList *llist, void *data);

/*
 * - uses llist->valueType->destruct if it is not NULL
 */
void tloSLListPopFront(TloSLList *llist);

/*
 * - deep copies data using llist->valueType->constructCopy if it is not null
 * - otherwise, uses memcpy
 */
TloError tloSLListPushBack(TloSLList *llist, const void *data);

/*
 * - assumes data points to an object whose memory was allocated by
 *   allocatorType->malloc
 * - takes ownership of the object
 */
TloError tloSLListMoveBack(TloSLList *llist, void *data);

bool tloSLLNodeIsValid(const TloSLLNode *node);
const TloSLLNode *tloSLLNodeHead(const TloSLList *llist);
TloSLLNode *tloSLLNodeMutableHead(TloSLList *llist);
const void *tloSLLNodeElement(const TloSLLNode *node);
void *tloSLLNodeMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
