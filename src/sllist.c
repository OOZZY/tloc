#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

bool tloSLListIsValid(const TloSLList *llist) {
  return llist && tloTypeIsValid(llist->valueType) &&
         tloAllocatorTypeIsValid(llist->allocatorType) &&
         ((llist->head == NULL) == (llist->size == 0)) &&
         ((llist->tail == NULL) == (llist->size == 0)) &&
         ((llist->head == NULL) == (llist->tail == NULL)) &&
         ((llist->size == 0 || llist->size == 1)
              ? (llist->head == llist->tail)
              : (llist->head != llist->tail));
}

TloError tloSLListConstruct(TloSLList *llist, const TloType *valueType,
                            const TloAllocatorType *allocatorType) {
  assert(llist);
  assert(tloTypeIsValid(valueType));

  if (!allocatorType) {
    allocatorType = &tloCStdLibAllocator;
  }

  assert(tloAllocatorTypeIsValid(allocatorType));

  llist->valueType = valueType;
  llist->allocatorType = allocatorType;
  llist->head = NULL;
  llist->tail = NULL;
  llist->size = 0;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloSLList *llist,
                                           const TloSLList *other) {
  for (TloSLLNode *node = other->head; node; node = node->next) {
    const void *element = node->bytes;
    if (tloSLListPushBack(llist, element) == TLO_ERROR) {
      tloSLListDestruct(llist);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other) {
  assert(llist);
  assert(tloSLListIsValid(other));

  if (tloSLListConstruct(llist, other->valueType, other->allocatorType) ==
      TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(llist, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void destructAllElementsAndFreeAllNodes(TloSLList *llist) {
  TloSLLNode *current = llist->head;

  while (current) {
    TloSLLNode *next = current->next;

    if (llist->valueType->destruct) {
      llist->valueType->destruct(current->bytes);
    }
    llist->allocatorType->free(current->bytes);
    llist->allocatorType->free(current);

    current = next;
  }
}

void tloSLListDestruct(TloSLList *llist) {
  if (!llist) {
    return;
  }

  assert(tloSLListIsValid(llist));

  if (!llist->head) {
    return;
  }

  destructAllElementsAndFreeAllNodes(llist);

  llist->head = NULL;
  llist->tail = NULL;
}

TloSLList *tloSLListMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType) {
  assert(tloTypeIsValid(valueType));

  if (!allocatorType) {
    allocatorType = &tloCStdLibAllocator;
  }

  assert(tloAllocatorTypeIsValid(allocatorType));

  TloSLList *llist = allocatorType->malloc(sizeof(*llist));
  if (!llist) {
    return NULL;
  }

  if (tloSLListConstruct(llist, valueType, allocatorType) == TLO_ERROR) {
    allocatorType->free(llist);
    return NULL;
  }

  return llist;
}

TloSLList *tloSLListMakeCopy(const TloSLList *other) {
  assert(tloSLListIsValid(other));

  TloSLList *llist = other->allocatorType->malloc(sizeof(*llist));
  if (!llist) {
    return NULL;
  }

  if (tloSLListConstructCopy(llist, other) == TLO_ERROR) {
    other->allocatorType->free(llist);
    return NULL;
  }

  return llist;
}

void tloSLListDelete(TloSLList *llist) {
  if (!llist) {
    return;
  }

  assert(tloSLListIsValid(llist));

  tloSLListDestruct(llist);
  TloFreeFunction free = llist->allocatorType->free;
  free(llist);
}

TloError tloSLListCopy(TloSLList *llist, const TloSLList *other) {
  assert(tloSLListIsValid(llist));
  assert(tloSLListIsValid(other));

  TloSLList copy;
  if (tloSLListConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  tloSLListDestruct(llist);
  memcpy(llist, &copy, sizeof(TloSLList));

  return TLO_SUCCESS;
}

const TloType *tloSLListValueType(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->valueType;
}

const TloAllocatorType *tloSLListAllocatorType(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->allocatorType;
}

size_t tloSLListSize(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->size;
}

bool tloSLListIsEmpty(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->size == 0;
}

const void *tloSLListFront(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));
  assert(!tloSLListIsEmpty(llist));

  return llist->head->bytes;
}

void *tloSLListMutableFront(TloSLList *llist) {
  assert(tloSLListIsValid(llist));
  assert(!tloSLListIsEmpty(llist));

  return llist->head->bytes;
}

const void *tloSLListBack(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));
  assert(!tloSLListIsEmpty(llist));

  return llist->tail->bytes;
}

void *tloSLListMutableBack(TloSLList *llist) {
  assert(tloSLListIsValid(llist));
  assert(!tloSLListIsEmpty(llist));

  return llist->tail->bytes;
}

static TloSLLNode *makeNodeWithCopiedData(TloSLList *llist, const void *data) {
  TloSLLNode *node = llist->allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = llist->allocatorType->malloc(llist->valueType->sizeOf);
  if (!node->bytes) {
    llist->allocatorType->free(node);
    return NULL;
  }

  if (llist->valueType->constructCopy) {
    if (llist->valueType->constructCopy(node->bytes, data) == TLO_ERROR) {
      llist->allocatorType->free(node->bytes);
      llist->allocatorType->free(node);
      return NULL;
    }
  } else {
    memcpy(node->bytes, data, llist->valueType->sizeOf);
  }

  node->next = NULL;
  return node;
}

static void pushFrontNode(TloSLList *llist, TloSLLNode *node) {
  if (!llist->head) {
    llist->head = node;
    llist->tail = llist->head;
  } else {
    node->next = llist->head;
    llist->head = node;
  }

  ++llist->size;
}

TloError tloSLListPushFront(TloSLList *llist, const void *data) {
  assert(tloSLListIsValid(llist));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

static TloSLLNode *makeNodeWithMovedData(TloSLList *llist, void *data) {
  TloSLLNode *node = llist->allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = data;
  node->next = NULL;
  return node;
}

TloError tloSLListMoveFront(TloSLList *llist, void *data) {
  assert(tloSLListIsValid(llist));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

void tloSLListPopFront(TloSLList *llist) {
  assert(tloSLListIsValid(llist));
  assert(!tloSLListIsEmpty(llist));

  TloSLLNode *frontNode = llist->head;
  llist->head = llist->head->next;
  if (llist->valueType->destruct) {
    llist->valueType->destruct(frontNode->bytes);
  }
  llist->allocatorType->free(frontNode->bytes);
  llist->allocatorType->free(frontNode);
  --llist->size;

  if (!llist->head) {
    llist->tail = NULL;
  }
}

static void pushBackNode(TloSLList *llist, TloSLLNode *node) {
  if (!llist->head) {
    llist->head = node;
    llist->tail = llist->head;
  } else {
    llist->tail->next = node;
    llist->tail = llist->tail->next;
  }

  ++llist->size;
}

TloError tloSLListPushBack(TloSLList *llist, const void *data) {
  assert(tloSLListIsValid(llist));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

TloError tloSLListMoveBack(TloSLList *llist, void *data) {
  assert(tloSLListIsValid(llist));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

bool tloSLLNodeIsValid(const TloSLLNode *node) { return node && node->bytes; }

const TloSLLNode *tloSLLNodeHead(const TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->head;
}

TloSLLNode *tloSLLNodeMutableHead(TloSLList *llist) {
  assert(tloSLListIsValid(llist));

  return llist->head;
}

const void *tloSLLNodeElement(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

void *tloSLLNodeMutableElement(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}

TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}
