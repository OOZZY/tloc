#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/util.h"

/*
 * * an implementation of a singly-linked list
 * * all operations are transactional
 */

typedef struct tloSLLNode {
  // private
  void *bytes;
  struct tloSLLNode *next;
} tloSLLNode;

typedef struct tloSLList {
  // private
  const tloType *type;
  const tloAllocator *allocator;
  tloSLLNode *head;
  tloSLLNode *tail;
  size_t size;
} tloSLList;

bool tloSLListIsValid(const tloSLList *list);
tloError tloSLListConstruct(tloSLList *list, const tloType *type,
                            const tloAllocator *allocator);
tloError tloSLListConstructCopy(tloSLList *list, const tloSLList *other);
void tloSLListDestruct(tloSLList *list);

/*
 * * allocator->malloc then tloSLListConstruct
 */
tloSLList *tloSLListMake(const tloType *type, const tloAllocator *allocator);

/*
 * * other->allocator->malloc then tloSLListConstructCopy
 */
tloSLList *tloSLListMakeCopy(const tloSLList *other);

/*
 * * tloSLListDestruct then list->allocator->free
 */
void tloSLListDelete(tloSLList *list);

tloError tloSLListCopy(tloSLList *list, const tloSLList *other);
const tloType *tloSLListGetType(const tloSLList *list);
const tloAllocator *tloSLListGetAllocator(const tloSLList *list);
size_t tloSLListGetSize(const tloSLList *list);
bool tloSLListIsEmpty(const tloSLList *list);
const void *tloSLListGetFront(const tloSLList *list);
void *tloSLListGetMutableFront(tloSLList *list);
const void *tloSLListGetBack(const tloSLList *list);
void *tloSLListGetMutableBack(tloSLList *list);

/*
 * * deep copies data using list->type->copy
 */
tloError tloSLListPushFront(tloSLList *list, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloSLListMoveFront(tloSLList *list, void *data);

void tloSLListPopFront(tloSLList *list);

/*
 * * deep copies data using list->type->copy
 */
tloError tloSLListPushBack(tloSLList *list, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
tloError tloSLListMoveBack(tloSLList *list, void *data);

bool tloSLLNodeIsValid(const tloSLLNode *node);
const tloSLLNode *tloSLLNodeGetHead(const tloSLList *list);
tloSLLNode *tloSLLNodeGetMutableHead(tloSLList *list);
const void *tloSLLNodeGetElement(const tloSLLNode *node);
void *tloSLLNodeGetMutableElement(tloSLLNode *node);
const tloSLLNode *tloSLLNodeGetNext(const tloSLLNode *node);
tloSLLNode *tloSLLNodeGetMutableNext(tloSLLNode *node);

#endif  // TLO_SLLIST_H
