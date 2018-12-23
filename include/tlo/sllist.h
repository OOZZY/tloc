#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/list.h"

/*
 * - singly-linked list
 */
typedef struct TloSLLNode {
  // private
  void *data;
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
                            const TloAllocator *allocator);

/*
 * - uses TloSLList's pushBack
 */
TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other);

/*
 * - uses given allocator's malloc then tloSLListConstruct
 */
TloSLList *tloSLListMake(const TloType *valueType,
                         const TloAllocator *allocator);

/*
 * - uses malloc of other's allocator then tloSLListConstructCopy
 */
TloSLList *tloSLListMakeCopy(const TloSLList *other);

/*
 * - uses tloSLListConstructCopy and TloSLList's destruct
 */
TloError tloSLListCopy(TloSLList *llist, const TloSLList *other);

const TloSLLNode *tloSLListHead(const TloSLList *llist);
TloSLLNode *tloSLListMutableHead(TloSLList *llist);

bool tloSLLNodeIsValid(const TloSLLNode *node);
const void *tloSLLNodeElement(const TloSLLNode *node);
void *tloSLLNodeMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
