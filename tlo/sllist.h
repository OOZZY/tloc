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
const TloType *tloSLListGetValueType(const TloSLList *list);
const TloAllocatorType *tloSLListGetAllocatorType(const TloSLList *list);
size_t tloSLListGetSize(const TloSLList *list);
bool tloSLListIsEmpty(const TloSLList *list);
const void *tloSLListGetFront(const TloSLList *list);
void *tloSLListGetMutableFront(TloSLList *list);
const void *tloSLListGetBack(const TloSLList *list);
void *tloSLListGetMutableBack(TloSLList *list);

/*
 * - deep copies data using list->valueType->copy
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
 * - deep copies data using list->valueType->copy
 */
TloError tloSLListPushBack(TloSLList *list, const void *data);

/*
 * - shallow copies data using memcpy then zeroes out data using memset
 */
TloError tloSLListMoveBack(TloSLList *list, void *data);

bool tloSLLNodeIsValid(const TloSLLNode *node);
const TloSLLNode *tloSLLNodeGetHead(const TloSLList *list);
TloSLLNode *tloSLLNodeGetMutableHead(TloSLList *list);
const void *tloSLLNodeGetElement(const TloSLLNode *node);
void *tloSLLNodeGetMutableElement(TloSLLNode *node);
const TloSLLNode *tloSLLNodeGetNext(const TloSLLNode *node);
TloSLLNode *tloSLLNodeGetMutableNext(TloSLLNode *node);

#endif  // TLO_SLLIST_H
