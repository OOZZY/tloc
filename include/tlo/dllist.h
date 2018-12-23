#ifndef TLO_DLLIST_H
#define TLO_DLLIST_H

#include "tlo/list.h"

/*
 * - doubly-linked list
 */
typedef struct TloDLLNode {
  // private
  void *data;
  struct TloDLLNode *next;
  struct TloDLLNode *prev;
} TloDLLNode;

typedef struct TloDLList {
  // public, use only for passing to tloList and tlovList functions
  TloList list;

  // private
  TloDLLNode *head;
  TloDLLNode *tail;
  size_t size;
} TloDLList;

TloError tloDLListConstruct(TloDLList *llist, const TloType *valueType,
                            const TloAllocator *allocator);

/*
 * - uses TloDLList's pushBack
 */
TloError tloDLListConstructCopy(TloDLList *llist, const TloDLList *other);

/*
 * - uses given allocator's malloc then tloDLListConstruct
 */
TloDLList *tloDLListMake(const TloType *valueType,
                         const TloAllocator *allocator);

/*
 * - uses malloc of other's allocator then tloDLListConstructCopy
 */
TloDLList *tloDLListMakeCopy(const TloDLList *other);

/*
 * - uses tloDLListConstructCopy and TloDLList's destruct
 */
TloError tloDLListCopy(TloDLList *llist, const TloDLList *other);

const TloDLLNode *tloDLListHead(const TloDLList *llist);
TloDLLNode *tloDLListMutableHead(TloDLList *llist);
const TloDLLNode *tloDLListTail(const TloDLList *llist);
TloDLLNode *tloDLListMutableTail(TloDLList *llist);

bool tloDLLNodeIsValid(const TloDLLNode *node);
const void *tloDLLNodeElement(const TloDLLNode *node);
void *tloDLLNodeMutableElement(TloDLLNode *node);
const TloDLLNode *tloDLLNodeNext(const TloDLLNode *node);
TloDLLNode *tloDLLNodeMutableNext(TloDLLNode *node);
const TloDLLNode *tloDLLNodePrev(const TloDLLNode *node);
TloDLLNode *tloDLLNodeMutablePrev(TloDLLNode *node);

#endif  // TLO_DLLIST_H
