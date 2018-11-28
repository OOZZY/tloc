#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

static bool isValid(const TloList *list) {
  const TloSLList *llist = (const TloSLList *)list;
  return tloListIsValid(list) &&
         ((llist->head == NULL) == (llist->size == 0)) &&
         ((llist->tail == NULL) == (llist->size == 0)) &&
         ((llist->head == NULL) == (llist->tail == NULL)) &&
         ((llist->size == 0 || llist->size == 1)
              ? (llist->head == llist->tail)
              : (llist->head != llist->tail));
}

static void destructAllElementsAndFreeAllNodes(TloSLList *llist) {
  TloSLLNode *current = llist->head;

  while (current) {
    TloSLLNode *next = current->next;

    if (llist->list.valueType->destruct) {
      llist->list.valueType->destruct(current->bytes);
    }
    llist->list.allocatorType->free(current->bytes);
    llist->list.allocatorType->free(current);

    current = next;
  }
}

static void destruct(TloList *list) {
  if (!list) {
    return;
  }

  assert(isValid(list));

  TloSLList *llist = (TloSLList *)list;
  if (!llist->head) {
    return;
  }

  destructAllElementsAndFreeAllNodes(llist);

  llist->head = NULL;
  llist->tail = NULL;
}

static size_t size(const TloList *list) {
  assert(isValid(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->size;
}

static bool isEmpty(const TloList *list) {
  assert(isValid(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->size == 0;
}

static const void *front(const TloList *list) {
  assert(isValid(list));
  assert(!isEmpty(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->head->bytes;
}

static void *mutableFront(TloList *list) {
  assert(isValid(list));
  assert(!isEmpty(list));

  TloSLList *llist = (TloSLList *)list;
  return llist->head->bytes;
}

static const void *back(const TloList *list) {
  assert(isValid(list));
  assert(!isEmpty(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->tail->bytes;
}

static void *mutableBack(TloList *list) {
  assert(isValid(list));
  assert(!isEmpty(list));

  TloSLList *llist = (TloSLList *)list;
  return llist->tail->bytes;
}

static TloSLLNode *makeNodeWithCopiedData(TloSLList *llist, const void *data) {
  TloSLLNode *node = llist->list.allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes =
      llist->list.allocatorType->malloc(llist->list.valueType->sizeOf);
  if (!node->bytes) {
    llist->list.allocatorType->free(node);
    return NULL;
  }

  if (llist->list.valueType->constructCopy) {
    if (llist->list.valueType->constructCopy(node->bytes, data) == TLO_ERROR) {
      llist->list.allocatorType->free(node->bytes);
      llist->list.allocatorType->free(node);
      return NULL;
    }
  } else {
    memcpy(node->bytes, data, llist->list.valueType->sizeOf);
  }

  node->next = NULL;
  return node;
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

static TloError pushBack(TloList *list, const void *data) {
  assert(isValid(list));
  assert(data);

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

static TloSLLNode *makeNodeWithMovedData(TloSLList *llist, void *data) {
  TloSLLNode *node = llist->list.allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = data;
  node->next = NULL;
  return node;
}

static TloError moveBack(TloList *list, void *data) {
  assert(isValid(list));
  assert(data);

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

static const TloListVTable vTable = {.isValid = isValid,
                                     .destruct = destruct,
                                     .size = size,
                                     .isEmpty = isEmpty,
                                     .front = front,
                                     .mutableFront = mutableFront,
                                     .back = back,
                                     .mutableBack = mutableBack,
                                     .pushBack = pushBack,
                                     .moveBack = moveBack};

TloError tloSLListConstruct(TloSLList *llist, const TloType *valueType,
                            const TloAllocatorType *allocatorType) {
  assert(llist);
  assert(tloTypeIsValid(valueType));
  assert(allocatorType == NULL || tloAllocatorTypeIsValid(allocatorType));

  tloListConstruct(&llist->list, &vTable, valueType, allocatorType);
  llist->head = NULL;
  llist->tail = NULL;
  llist->size = 0;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloSLList *llist,
                                           const TloSLList *other) {
  for (TloSLLNode *node = other->head; node; node = node->next) {
    const void *element = node->bytes;
    if (pushBack(&llist->list, element) == TLO_ERROR) {
      destruct(&llist->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other) {
  assert(llist);
  assert(isValid(&other->list));

  if (tloSLListConstruct(llist, other->list.valueType,
                         other->list.allocatorType) == TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(llist, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
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
  assert(isValid(&other->list));

  TloSLList *llist = other->list.allocatorType->malloc(sizeof(*llist));
  if (!llist) {
    return NULL;
  }

  if (tloSLListConstructCopy(llist, other) == TLO_ERROR) {
    other->list.allocatorType->free(llist);
    return NULL;
  }

  return llist;
}

TloError tloSLListCopy(TloSLList *llist, const TloSLList *other) {
  assert(isValid(&llist->list));
  assert(isValid(&other->list));

  TloSLList copy;
  if (tloSLListConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  destruct(&llist->list);
  memcpy(llist, &copy, sizeof(TloSLList));

  return TLO_SUCCESS;
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
  assert(isValid(&llist->list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

TloError tloSLListMoveFront(TloSLList *llist, void *data) {
  assert(isValid(&llist->list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

void tloSLListPopFront(TloSLList *llist) {
  assert(isValid(&llist->list));
  assert(!isEmpty(&llist->list));

  TloSLLNode *frontNode = llist->head;
  llist->head = llist->head->next;
  if (llist->list.valueType->destruct) {
    llist->list.valueType->destruct(frontNode->bytes);
  }
  llist->list.allocatorType->free(frontNode->bytes);
  llist->list.allocatorType->free(frontNode);
  --llist->size;

  if (!llist->head) {
    llist->tail = NULL;
  }
}

bool tloSLLNodeIsValid(const TloSLLNode *node) { return node && node->bytes; }

const TloSLLNode *tloSLLNodeHead(const TloSLList *llist) {
  assert(isValid(&llist->list));

  return llist->head;
}

TloSLLNode *tloSLLNodeMutableHead(TloSLList *llist) {
  assert(isValid(&llist->list));

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
