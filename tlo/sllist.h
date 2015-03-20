#ifndef TLO_SLLIST_H
#define TLO_SLLIST_H

#include "tlo/util.h"

/*
 * * an implementation of a singly-linked list
 * * all operations are transactional
 */

typedef struct tloSLListNode {
  void *bytes;
  struct tloSLListNode *next;
} tloSLListNode;

typedef struct tloSLList {
  const tloType *type;
  const tloAllocator *allocator;
  tloSLListNode *head;
  tloSLListNode *tail;
  size_t size;
} tloSLList;

bool tloSLListIsValid(const tloSLList *list);
int tloSLListConstruct(tloSLList *list, const tloType *type,
                       const tloAllocator *allocator);
void tloSLListDestruct(tloSLList *list);

/*
 * * allocator->malloc then tloSLListConstruct
 */
tloSLList *tloSLListMake(const tloType *type, const tloAllocator *allocator);

/*
 * * tloSLListDestruct then list->allocator->free
 */
void tloSLListDelete(tloSLList *list);

const tloType *tloSLListGetType(const tloSLList *list);
const tloAllocator *tloSLListGetAllocator(const tloSLList *list);
size_t tloSLListGetSize(const tloSLList *list);
bool tloSLListIsEmpty(const tloSLList *list);
const void *tloSLListGetFrontReadOnly(const tloSLList *list);
void *tloSLListGetFrontReadWrite(tloSLList *list);
const void *tloSLListGetBackReadOnly(const tloSLList *list);
void *tloSLListGetBackReadWrite(tloSLList *list);

/*
 * * deep copies data using list->type->copy
 */
int tloSLListPushBackCopy(tloSLList *list, const void *data);

/*
 * * shallow copies data using memcpy then zeroes out data using memset
 */
int tloSLListPushBackMove(tloSLList *list, void *data);

#endif // TLO_SLLIST_H
