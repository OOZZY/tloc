#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

static bool sllistIsValid(const TloList *list) {
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
      llist->list.valueType->destruct(current->data);
    }
    llist->list.allocatorType->free(current->data);
    llist->list.allocatorType->free(current);

    current = next;
  }
}

static void sllistDestruct(TloList *list) {
  if (!list) {
    return;
  }

  assert(sllistIsValid(list));

  TloSLList *llist = (TloSLList *)list;
  if (!llist->head) {
    return;
  }

  destructAllElementsAndFreeAllNodes(llist);

  llist->head = NULL;
  llist->tail = NULL;
}

static size_t sllistSize(const TloList *list) {
  assert(sllistIsValid(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->size;
}

static bool sllistIsEmpty(const TloList *list) {
  assert(sllistIsValid(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->size == 0;
}

static const void *sllistFront(const TloList *list) {
  assert(sllistIsValid(list));
  assert(!sllistIsEmpty(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->head->data;
}

static void *sllistMutableFront(TloList *list) {
  assert(sllistIsValid(list));
  assert(!sllistIsEmpty(list));

  TloSLList *llist = (TloSLList *)list;
  return llist->head->data;
}

static const void *sllistBack(const TloList *list) {
  assert(sllistIsValid(list));
  assert(!sllistIsEmpty(list));

  const TloSLList *llist = (const TloSLList *)list;
  return llist->tail->data;
}

static void *sllistMutableBack(TloList *list) {
  assert(sllistIsValid(list));
  assert(!sllistIsEmpty(list));

  TloSLList *llist = (TloSLList *)list;
  return llist->tail->data;
}

static TloSLLNode *makeNodeWithCopiedData(TloSLList *llist, const void *data) {
  TloSLLNode *node = llist->list.allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->data = llist->list.allocatorType->malloc(llist->list.valueType->sizeOf);
  if (!node->data) {
    llist->list.allocatorType->free(node);
    return NULL;
  }

  if (llist->list.valueType->constructCopy) {
    if (llist->list.valueType->constructCopy(node->data, data) == TLO_ERROR) {
      llist->list.allocatorType->free(node->data);
      llist->list.allocatorType->free(node);
      return NULL;
    }
  } else {
    memcpy(node->data, data, llist->list.valueType->sizeOf);
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

static TloError sllistPushBack(TloList *list, const void *data) {
  assert(sllistIsValid(list));
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

  node->data = data;
  node->next = NULL;
  return node;
}

static TloError sllistMoveBack(TloList *list, void *data) {
  assert(sllistIsValid(list));
  assert(data);

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

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

static TloError sllistPushFront(TloList *list, const void *data) {
  assert(sllistIsValid(list));
  assert(data);

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

static TloError sllistMoveFront(TloList *list, void *data) {
  assert(sllistIsValid(list));
  assert(data);

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

static void sllistPopFront(TloList *list) {
  assert(sllistIsValid(list));
  assert(!sllistIsEmpty(list));

  TloSLList *llist = (TloSLList *)list;
  TloSLLNode *frontNode = llist->head;
  llist->head = llist->head->next;
  if (llist->list.valueType->destruct) {
    llist->list.valueType->destruct(frontNode->data);
  }
  llist->list.allocatorType->free(frontNode->data);
  llist->list.allocatorType->free(frontNode);
  --llist->size;

  if (!llist->head) {
    llist->tail = NULL;
  }
}

static const TloListVTable vTable = {.type = "TloSLList",
                                     .isValid = sllistIsValid,
                                     .destruct = sllistDestruct,
                                     .size = sllistSize,
                                     .isEmpty = sllistIsEmpty,
                                     .front = sllistFront,
                                     .mutableFront = sllistMutableFront,
                                     .back = sllistBack,
                                     .mutableBack = sllistMutableBack,
                                     .pushBack = sllistPushBack,
                                     .moveBack = sllistMoveBack,
                                     .pushFront = sllistPushFront,
                                     .moveFront = sllistMoveFront,
                                     .popFront = sllistPopFront};

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
    const void *element = node->data;
    if (sllistPushBack(&llist->list, element) == TLO_ERROR) {
      sllistDestruct(&llist->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloSLListConstructCopy(TloSLList *llist, const TloSLList *other) {
  assert(llist);
  assert(sllistIsValid(&other->list));

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
  assert(allocatorType == NULL || tloAllocatorTypeIsValid(allocatorType));

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
  assert(sllistIsValid(&other->list));

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
  assert(sllistIsValid(&llist->list));
  assert(sllistIsValid(&other->list));

  TloSLList copy;
  if (tloSLListConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  sllistDestruct(&llist->list);
  memcpy(llist, &copy, sizeof(TloSLList));

  return TLO_SUCCESS;
}

bool tloSLLNodeIsValid(const TloSLLNode *node) { return node && node->data; }

const TloSLLNode *tloSLLNodeHead(const TloSLList *llist) {
  assert(sllistIsValid(&llist->list));

  return llist->head;
}

TloSLLNode *tloSLLNodeMutableHead(TloSLList *llist) {
  assert(sllistIsValid(&llist->list));

  return llist->head;
}

const void *tloSLLNodeElement(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->data;
}

void *tloSLLNodeMutableElement(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->data;
}

const TloSLLNode *tloSLLNodeNext(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}

TloSLLNode *tloSLLNodeMutableNext(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}
