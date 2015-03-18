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
  size_t size;
} tloSLList;

bool tloSLListIsValid(const tloSLList *list);
int tloSLListConstruct(tloSLList *list, const tloType *type,
                       const tloAllocator *allocator);
void tloSLListDestruct(tloSLList *list);
const tloType *tloSLListGetType(const tloSLList *list);
const tloAllocator *tloSLListGetAllocator(const tloSLList *list);
size_t tloSLListGetSize(const tloSLList *list);
bool tloSLListIsEmpty(const tloSLList *list);

#endif // TLO_SLLIST_H
