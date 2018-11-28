#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/list.h"

/*
 * - singly-linked list
 */
typedef struct TloSLLNode {
  // private
  void *bytes;
  struct TloSLLNode *next;
} TloSLLNode;

typedef struct TloSLList {
  // public, use only for passing to tloList and tlovList functions
  TloList list;

  // private
  TloSLLNode *head;
  TloSLLNode *tail;
  size_t size;
} TloSLList;

TloError tloSLListConstruct(TloSLList *llist, const TloType *valueType,
                            const TloAllocatorType *allocatorType);

/*
 * - uses tloSLListPushBack
 */
TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other);

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
 * - uses tloSLListConstructCopy and tloSLListDestruct
 */
TloError tloSLListCopy(TloSLList *llist, const TloSLList *other);

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

bool tloSLLNodeIsValid(const TloSLLNode *node);
const TloSLLNode *tloSLLNodeHead(const TloSLList *llist);
TloSLLNode *tloSLLNodeMutableHead(TloSLList *llist);
const void *tloSLLNodeElement(const TloSLLNode *node);
void *tloSLLNodeMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
