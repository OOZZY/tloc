#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

bool tloSLListIsValid(const TloSLList *list) {
  return list && tloTypeIsValid(list->valueType) &&
         tloAllocatorTypeIsValid(list->allocatorType) &&
         ((list->head == NULL) == (list->size == 0)) &&
         ((list->tail == NULL) == (list->size == 0)) &&
         ((list->head == NULL) == (list->tail == NULL)) &&
         ((list->size == 0 || list->size == 1) ? (list->head == list->tail)
                                               : (list->head != list->tail));
}

TloError tloSLListConstruct(TloSLList *list, const TloType *valueType,
                            const TloAllocatorType *allocatorType) {
  assert(list);
  assert(tloTypeIsValid(valueType));
  assert(tloAllocatorTypeIsValid(allocatorType));

  list->valueType = valueType;
  list->allocatorType = allocatorType;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return TLO_SUCCESS;
}

static TloError pushBackAllElementsOfOther(TloSLList *list,
                                           const TloSLList *other) {
  for (TloSLLNode *node = other->head; node; node = node->next) {
    const void *element = node->bytes;
    if (tloSLListPushBack(list, element) == TLO_ERROR) {
      tloSLListDestruct(list);
      return TLO_ERROR;
    }
  }

  return TLO_SUCCESS;
}

TloError tloSLListConstructCopy(TloSLList *list, const TloSLList *other) {
  assert(list);
  assert(tloSLListIsValid(other));

  if (tloSLListConstruct(list, other->valueType, other->allocatorType) ==
      TLO_ERROR) {
    return TLO_ERROR;
  }

  if (pushBackAllElementsOfOther(list, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  return TLO_SUCCESS;
}

static void destructAllElementsAndFreeAllNodes(TloSLList *list) {
  TloSLLNode *current = list->head;

  while (current) {
    TloSLLNode *next = current->next;

    list->valueType->destruct(current->bytes);
    list->allocatorType->free(current->bytes);
    list->allocatorType->free(current);

    current = next;
  }
}

void tloSLListDestruct(TloSLList *list) {
  if (!list) {
    return;
  }

  assert(tloSLListIsValid(list));

  if (!list->head) {
    return;
  }

  destructAllElementsAndFreeAllNodes(list);

  list->head = NULL;
  list->tail = NULL;
}

TloSLList *tloSLListMake(const TloType *valueType,
                         const TloAllocatorType *allocatorType) {
  assert(tloTypeIsValid(valueType));
  assert(tloAllocatorTypeIsValid(allocatorType));

  TloSLList *list = allocatorType->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstruct(list, valueType, allocatorType) == TLO_ERROR) {
    allocatorType->free(list);
    return NULL;
  }

  return list;
}

TloSLList *tloSLListMakeCopy(const TloSLList *other) {
  assert(tloSLListIsValid(other));

  TloSLList *list = other->allocatorType->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstructCopy(list, other) == TLO_ERROR) {
    other->allocatorType->free(list);
    return NULL;
  }

  return list;
}

void tloSLListDelete(TloSLList *list) {
  if (!list) {
    return;
  }

  assert(tloSLListIsValid(list));

  tloSLListDestruct(list);
  TloFreeFunction free = list->allocatorType->free;
  free(list);
}

TloError tloSLListCopy(TloSLList *list, const TloSLList *other) {
  assert(tloSLListIsValid(list));
  assert(tloSLListIsValid(other));

  TloSLList copy;
  if (tloSLListConstructCopy(&copy, other) == TLO_ERROR) {
    return TLO_ERROR;
  }

  tloSLListDestruct(list);
  memcpy(list, &copy, sizeof(TloSLList));

  return TLO_SUCCESS;
}

const TloType *tloSLListGetValueType(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->valueType;
}

const TloAllocatorType *tloSLListGetAllocatorType(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->allocatorType;
}

size_t tloSLListGetSize(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->size;
}

bool tloSLListIsEmpty(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->size == 0;
}

const void *tloSLListGetFront(const TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->head->bytes;
}

void *tloSLListGetMutableFront(TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->head->bytes;
}

const void *tloSLListGetBack(const TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
}

void *tloSLListGetMutableBack(TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
}

static TloSLLNode *makeNodeWithCopiedData(TloSLList *list, const void *data) {
  TloSLLNode *node = list->allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = list->allocatorType->malloc(list->valueType->sizeOf);
  if (!node->bytes) {
    list->allocatorType->free(node);
    return NULL;
  }

  if (list->valueType->constructCopy(node->bytes, data) == TLO_ERROR) {
    list->allocatorType->free(node->bytes);
    list->allocatorType->free(node);
    return NULL;
  }

  node->next = NULL;
  return node;
}

static void pushFrontNode(TloSLList *list, TloSLLNode *node) {
  if (!list->head) {
    list->head = node;
    list->tail = list->head;
  } else {
    node->next = list->head;
    list->head = node;
  }

  ++list->size;
}

TloError tloSLListPushFront(TloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(list, newNode);

  return TLO_SUCCESS;
}

static TloSLLNode *makeNodeWithMovedData(TloSLList *list, void *data) {
  TloSLLNode *node = list->allocatorType->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = list->allocatorType->malloc(list->valueType->sizeOf);
  if (!node->bytes) {
    list->allocatorType->free(node);
    return NULL;
  }

  memcpy(node->bytes, data, list->valueType->sizeOf);
  memset(data, 0, list->valueType->sizeOf);

  node->next = NULL;
  return node;
}

TloError tloSLListMoveFront(TloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushFrontNode(list, newNode);

  return TLO_SUCCESS;
}

void tloSLListPopFront(TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  TloSLLNode *frontNode = list->head;
  list->head = list->head->next;
  list->valueType->destruct(frontNode->bytes);
  list->allocatorType->free(frontNode->bytes);
  list->allocatorType->free(frontNode);
  --list->size;

  if (!list->head) {
    list->tail = NULL;
  }
}

static void pushBackNode(TloSLList *list, TloSLLNode *node) {
  if (!list->head) {
    list->head = node;
    list->tail = list->head;
  } else {
    list->tail->next = node;
    list->tail = list->tail->next;
  }

  ++list->size;
}

TloError tloSLListPushBack(TloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(list, newNode);

  return TLO_SUCCESS;
}

TloError tloSLListMoveBack(TloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return TLO_ERROR;
  }

  pushBackNode(list, newNode);

  return TLO_SUCCESS;
}

bool tloSLLNodeIsValid(const TloSLLNode *node) { return node && node->bytes; }

const TloSLLNode *tloSLLNodeGetHead(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

TloSLLNode *tloSLLNodeGetMutableHead(TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

const void *tloSLLNodeGetElement(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

void *tloSLLNodeGetMutableElement(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

const TloSLLNode *tloSLLNodeGetNext(const TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}

TloSLLNode *tloSLLNodeGetMutableNext(TloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}
