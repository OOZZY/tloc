#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/util.h"

/*
 * * an implementation of a singly-linked list
 * * all operations are transactional
 */

typedef struct tloSLLNode {
  void *bytes;
  struct tloSLLNode *next;
} tloSLLNode;

typedef struct tloSLList {
  const tloType *type;
  const tloAllocator *allocator;
  tloSLLNode *head;
  tloSLLNode *tail;
  size_t size;
} tloSLList;

bool tloSLListIsValid(const tloSLList *list);
int tloSLListConstruct(tloSLList *list, const tloType *type,
                       const tloAllocator *allocator);
int tloSLListConstructCopy(tloSLList *list, const tloSLList *other);
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

int tloSLListCopy(tloSLList *list, const tloSLList *other);
const tloType *tloSLListGetType(const tloSLList *list);
const tloAllocator *tloSLListGetAllocator(const tloSLList *list);
size_t tloSLListGetSize(const tloSLList *list);
bool tloSLListIsEmpty(const tloSLList *list);
const void *tloSLListGetFront(const tloSLList *list);
void *tloSLListGetFrontRW(tloSLList *list);
const void *tloSLListGetBack(const tloSLList *list);
void *tloSLListGetBackRW(tloSLList *list);

/*
 * * deep copies data using list->type->copy
 */
int tloSLListPushFront(tloSLList *list, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
int tloSLListPushFrontMove(tloSLList *list, void *data);

void tloSLListPopFront(tloSLList *list);

/*
 * * deep copies data using list->type->copy
 */
int tloSLListPushBack(tloSLList *list, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
int tloSLListPushBackMove(tloSLList *list, void *data);

bool tloSLLNodeIsValid(const tloSLLNode *node);
const tloSLLNode *tloSLLNodeGetHead(const tloSLList *list);
tloSLLNode *tloSLLNodeGetHeadRW(tloSLList *list);
const void *tloSLLNodeGetElement(const tloSLLNode *node);
void *tloSLLNodeGetElementRW(tloSLLNode *node);
const tloSLLNode *tloSLLNodeGetNext(const tloSLLNode *node);
tloSLLNode *tloSLLNodeGetNextRW(tloSLLNode *node);

#endif // TLO_SLLIST_H
