#include "tlo/dllist.h"
#include <assert.h>
#include <string.h>

static bool dllistIsValid(const TloList *list) {
  const TloDLList *llist = (const TloDLList *)list;
  return tloListIsValid(list) &&
         ((llist->head == NULL) == (llist->size == 0)) &&
         ((llist->tail == NULL) == (llist->size == 0)) &&
         ((llist->head == NULL) == (llist->tail == NULL)) &&
         ((llist->size == 0 || llist->size == 1)
              ? (llist->head == llist->tail)
              : (llist->head != llist->tail));
}

static void destructAllElementsAndFreeAllNodes(TloDLList *llist) {
  TloDLLNode *current = llist->head;

  while (current) {
    TloDLLNode *next = current->next;

    tloTypeDestruct(llist->list.valueType, current->data);
    llist->list.allocator->free(current->data);
    llist->list.allocator->free(current);

    current = next;
  }
}

static void dllistDestruct(TloList *list) {
  if (!list) {
    return;
  }

  assert(dllistIsValid(list));

  TloDLList *llist = (TloDLList *)list;
  if (!llist->head) {
    return;
  }

  destructAllElementsAndFreeAllNodes(llist);

  llist->head = NULL;
  llist->tail = NULL;
}

static size_t dllistSize(const TloList *list) {
  assert(dllistIsValid(list));

  const TloDLList *llist = (const TloDLList *)list;
  return llist->size;
}

static bool dllistIsEmpty(const TloList *list) {
  assert(dllistIsValid(list));

  const TloDLList *llist = (const TloDLList *)list;
  return llist->size == 0;
}

static const void *dllistFront(const TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  const TloDLList *llist = (const TloDLList *)list;
  return llist->head->data;
}

static void *dllistMutableFront(TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  TloDLList *llist = (TloDLList *)list;
  return llist->head->data;
}

static const void *dllistBack(const TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  const TloDLList *llist = (const TloDLList *)list;
  return llist->tail->data;
}

static void *dllistMutableBack(TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  TloDLList *llist = (TloDLList *)list;
  return llist->tail->data;
}

static TloDLLNode *makeNodeWithCopiedData(TloDLList *llist, const void *data) {
  TloDLLNode *node = llist->list.allocator->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->data = llist->list.allocator->malloc(llist->list.valueType->size);
  if (!node->data) {
    llist->list.allocator->free(node);
    return NULL;
  }

  if (tloTypeConstructCopy(llist->list.valueType, node->data, data) ==
      TLO_ERROR) {
    llist->list.allocator->free(node->data);
    llist->list.allocator->free(node);
    return NULL;
  }

  node->next = NULL;
  node->prev = NULL;
  return node;
}

static void pushBackNode(TloDLList *llist, TloDLLNode *node) {
  if (!llist->head) {
    llist->head = node;
    llist->tail = llist->head;
  } else {
    llist->tail->next = node;
    node->prev = llist->tail;
    llist->tail = llist->tail->next;
  }

  ++llist->size;
}

static TloError dllistPushBack(TloList *list, const void *data) {
  assert(dllistIsValid(list));
  assert(data);

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

static TloDLLNode *makeNodeWithMovedData(TloDLList *llist, void *data) {
  TloDLLNode *node = llist->list.allocator->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

static TloError dllistMoveBack(TloList *list, void *data) {
  assert(dllistIsValid(list));
  assert(data);

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(llist, newNode);

  return TLO_SUCCESS;
}

static void pushFrontNode(TloDLList *llist, TloDLLNode *node) {
  if (!llist->head) {
    llist->head = node;
    llist->tail = llist->head;
  } else {
    node->next = llist->head;
    llist->head->prev = node;
    llist->head = node;
  }

  ++llist->size;
}

static TloError dllistPushFront(TloList *list, const void *data) {
  assert(dllistIsValid(list));
  assert(data);

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *newNode = makeNodeWithCopiedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

static TloError dllistMoveFront(TloList *list, void *data) {
  assert(dllistIsValid(list));
  assert(data);

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *newNode = makeNodeWithMovedData(llist, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(llist, newNode);

  return TLO_SUCCESS;
}

static void dllistPopFront(TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *frontNode = llist->head;
  llist->head = llist->head->next;
  tloTypeDestruct(llist->list.valueType, frontNode->data);
  llist->list.allocator->free(frontNode->data);
  llist->list.allocator->free(frontNode);
  --llist->size;

  if (!llist->head) {
    llist->tail = NULL;
  } else {
    llist->head->prev = NULL;
  }
}

static void dllistPopBack(TloList *list) {
  assert(dllistIsValid(list));
  assert(!dllistIsEmpty(list));

  TloDLList *llist = (TloDLList *)list;
  TloDLLNode *backNode = llist->tail;
  llist->tail = llist->tail->prev;
  tloTypeDestruct(llist->list.valueType, backNode->data);
  llist->list.allocator->free(backNode->data);
  llist->list.allocator->free(backNode);
  --llist->size;

  if (!llist->tail) {
    llist->head = NULL;
  } else {
    llist->tail->next = NULL;
  }
}

static const TloListVTable vTable = {.type = "TloDLList",
                                     .isValid = dllistIsValid,
                                     .destruct = dllistDestruct,
                                     .size = dllistSize,
                                     .isEmpty = dllistIsEmpty,
                                     .front = dllistFront,
                                     .mutableFront = dllistMutableFront,
                                     .back = dllistBack,
                                     .mutableBack = dllistMutableBack,
                                     .pushBack = dllistPushBack,
                                     .moveBack = dllistMoveBack,
                                     .pushFront = dllistPushFront,
                                     .moveFront = dllistMoveFront,
                                     .popFront = dllistPopFront,
                                     .popBack = dllistPopBack};

void tloDLListConstruct(TloDLList *llist, const TloType *valueType,
                        const TloAllocator *allocator) {
  assert(llist);
  assert(tloTypeIsValid(valueType));
  assert(allocator == NULL || tloAllocatorIsValid(allocator));

  tloListConstruct(&llist->list, &vTable, valueType, allocator);
  llist->head = NULL;
  llist->tail = NULL;
  llist->size = 0;
}

static TloError pushBackAllElementsOfOther(TloDLList *llist,
                                           const TloDLList *other) {
  for (TloDLLNode *node = other->head; node; node = node->next) {
    const void *element = node->data;
    if (dllistPushBack(&llist->list, element) == TLO_ERROR) {
      dllistDestruct(&llist->list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloDLListConstructCopy(TloDLList *llist, const TloDLList *other) {
  assert(llist);
  assert(dllistIsValid(&other->list));

  tloDLListConstruct(llist, other->list.valueType, other->list.allocator);

  if (pushBackAllElementsOfOther(llist, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

TloDLList *tloDLListMake(const TloType *valueType,
                         const TloAllocator *allocator) {
  assert(tloTypeIsValid(valueType));

  if (!allocator) {
    allocator = &tloCStdLibAllocator;
  }

  assert(tloAllocatorIsValid(allocator));

  TloDLList *llist = allocator->malloc(sizeof(*llist));
  if (!llist) {
    return NULL;
  }

  tloDLListConstruct(llist, valueType, allocator);

  return llist;
}

TloDLList *tloDLListMakeCopy(const TloDLList *other) {
  assert(dllistIsValid(&other->list));

  TloDLList *llist = other->list.allocator->malloc(sizeof(*llist));
  if (!llist) {
    return NULL;
  }

  if (tloDLListConstructCopy(llist, other) == TLO_ERROR) {
    other->list.allocator->free(llist);
    return NULL;
  }

  return llist;
}

TloError tloDLListCopy(TloDLList *llist, const TloDLList *other) {
  assert(dllistIsValid(&llist->list));
  assert(dllistIsValid(&other->list));

  TloDLList copy;
  if (tloDLListConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  dllistDestruct(&llist->list);
  memcpy(llist, &copy, sizeof(TloDLList));

  return TLO_SUCCESS;
}

const TloDLLNode *tloDLListHead(const TloDLList *llist) {
  assert(dllistIsValid(&llist->list));

  return llist->head;
}

TloDLLNode *tloDLListMutableHead(TloDLList *llist) {
  assert(dllistIsValid(&llist->list));

  return llist->head;
}

const TloDLLNode *tloDLListTail(const TloDLList *llist) {
  assert(dllistIsValid(&llist->list));

  return llist->tail;
}

TloDLLNode *tloDLListMutableTail(TloDLList *llist) {
  assert(dllistIsValid(&llist->list));

  return llist->tail;
}

static bool dllnodeIsValid(const TloDLLNode *node) {
  return node && node->data;
}

const void *tloDLLNodeElement(const TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->data;
}

void *tloDLLNodeMutableElement(TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->data;
}

const TloDLLNode *tloDLLNodeNext(const TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->next;
}

TloDLLNode *tloDLLNodeMutableNext(TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->next;
}

const TloDLLNode *tloDLLNodePrev(const TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->prev;
}

TloDLLNode *tloDLLNodeMutablePrev(TloDLLNode *node) {
  assert(dllnodeIsValid(node));

  return node->prev;
}
