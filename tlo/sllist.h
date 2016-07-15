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
  const TloType *type;
  const TloAllocator *allocator;
  TloSLLNode *head;
  TloSLLNode *tail;
  size_t size;
} TloSLList;

bool tloSLListIsValid(const TloSLList *list);
tloError tloSLListConstruct(TloSLList *list, const TloType *type,
                            const TloAllocator *allocator);
tloError tloSLListConstructCopy(TloSLList *list, const TloSLList *other);
void tloSLListDestruct(TloSLList *list);

/*
 * - allocator->malloc then tloSLListConstruct
 */
TloSLList *tloSLListMake(const TloType *type, const TloAllocator *allocator);

/*
 * - other->allocator->malloc then tloSLListConstructCopy
 */
TloSLList *tloSLListMakeCopy(const TloSLList *other);

/*
 * - tloSLListDestruct then list->allocator->free
 */
void tloSLListDelete(TloSLList *list);

tloError tloSLListCopy(TloSLList *list, const TloSLList *other);
const TloType *tloSLListGetType(const TloSLList *list);
const TloAllocator *tloSLListGetAllocator(const TloSLList *list);
size_t tloSLListGetSize(const TloSLList *list);
bool tloSLListIsEmpty(const TloSLList *list);
const void *tloSLListGetFront(const TloSLList *list);
void *tloSLListGetMutableFront(TloSLList *list);
const void *tloSLListGetBack(const TloSLList *list);
void *tloSLListGetMutableBack(TloSLList *list);

/*
 * - deep copies data using list->type->copy
 */
tloError tloSLListPushFront(TloSLList *list, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloSLListMoveFront(TloSLList *list, void *data);

void tloSLListPopFront(TloSLList *list);

/*
 * - deep copies data using list->type->copy
 */
tloError tloSLListPushBack(TloSLList *list, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloSLListMoveBack(TloSLList *list, void *data);

bool tloSLLNodeIsValid(const TloSLLNode *node);
const TloSLLNode *tloSLLNodeGetHead(const TloSLList *list);
TloSLLNode *tloSLLNodeGetMutableHead(TloSLList *list);
const void *tloSLLNodeGetElement(const TloSLLNode *node);
void *tloSLLNodeGetMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeGetNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeGetMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
