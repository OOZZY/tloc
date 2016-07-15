#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

static void destructAllElementsAndFreeAllNodes(TloSLList *list) {
  TloSLLNode *current = list->head;

  while (current) {
    TloSLLNode *next = current->next;

    list->type->destruct(current->bytes);
    list->allocator->free(current->bytes);
    list->allocator->free(current);

    current = next;
  }
}

static TloSLLNode *makeNodeWithCopiedData(TloSLList *list, const void *data) {
  TloSLLNode *node = list->allocator->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = list->allocator->malloc(list->type->sizeOf);
  if (!node->bytes) {
    list->allocator->free(node);
    return NULL;
  }

  if (list->type->constructCopy(node->bytes, data)) {
    list->allocator->free(node->bytes);
    list->allocator->free(node);
    return NULL;
  }

  node->next = NULL;
  return node;
}

static TloSLLNode *makeNodeWithMovedData(TloSLList *list, void *data) {
  TloSLLNode *node = list->allocator->malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->bytes = list->allocator->malloc(list->type->sizeOf);
  if (!node->bytes) {
    list->allocator->free(node);
    return NULL;
  }

  memcpy(node->bytes, data, list->type->sizeOf);
  memset(data, 0, list->type->sizeOf);

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

static tloError pushBackAllElementsOfOther(TloSLList *list,
                                           const TloSLList *other) {
  for (TloSLLNode *node = other->head; node; node = node->next) {
    const void *element = node->bytes;
    if (tloSLListPushBack(list, element) == -1) {
      tloSLListDestruct(list);
      return -1;
    }
  }

  return 0;
}

bool tloSLListIsValid(const TloSLList *list) {
  return list && tloTypeIsValid(list->type) &&
         tloAllocatorIsValid(list->allocator) &&
         ((list->head == NULL) == (list->size == 0)) &&
         ((list->tail == NULL) == (list->size == 0)) &&
         ((list->head == NULL) == (list->tail == NULL)) &&
         ((list->size == 0 || list->size == 1) ? (list->head == list->tail)
                                               : (list->head != list->tail));
}

tloError tloSLListConstruct(TloSLList *list, const TloType *type,
                            const TloAllocator *allocator) {
  assert(list);
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  list->type = type;
  list->allocator = allocator;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return 0;
}

tloError tloSLListConstructCopy(TloSLList *list, const TloSLList *other) {
  assert(list);
  assert(tloSLListIsValid(other));

  if (tloSLListConstruct(list, other->type, other->allocator) == -1) {
    return -1;
  }

  if (pushBackAllElementsOfOther(list, other) == -1) {
    return -1;
  }

  return 0;
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

TloSLList *tloSLListMake(const TloType *type, const TloAllocator *allocator) {
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  TloSLList *list = allocator->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstruct(list, type, allocator)) {
    allocator->free(list);
    return NULL;
  }

  return list;
}

TloSLList *tloSLListMakeCopy(const TloSLList *other) {
  assert(tloSLListIsValid(other));

  TloSLList *list = other->allocator->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstructCopy(list, other)) {
    other->allocator->free(list);
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
  tloFreeFunction free = list->allocator->free;
  free(list);
}

tloError tloSLListCopy(TloSLList *list, const TloSLList *other) {
  assert(tloSLListIsValid(list));
  assert(tloSLListIsValid(other));

  TloSLList copy;
  if (tloSLListConstructCopy(&copy, other) == -1) {
    return -1;
  }

  tloSLListDestruct(list);
  memcpy(list, &copy, sizeof(TloSLList));

  return 0;
}

const TloType *tloSLListGetType(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->type;
}

const TloAllocator *tloSLListGetAllocator(const TloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->allocator;
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

tloError tloSLListPushFront(TloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return -1;
  }

  pushFrontNode(list, newNode);

  return 0;
}

tloError tloSLListMoveFront(TloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return -1;
  }

  pushFrontNode(list, newNode);

  return 0;
}

void tloSLListPopFront(TloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  TloSLLNode *frontNode = list->head;
  list->head = list->head->next;
  list->type->destruct(frontNode->bytes);
  list->allocator->free(frontNode->bytes);
  list->allocator->free(frontNode);
  --list->size;

  if (!list->head) {
    list->tail = NULL;
  }
}

tloError tloSLListPushBack(TloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return -1;
  }

  pushBackNode(list, newNode);

  return 0;
}

tloError tloSLListMoveBack(TloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  TloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return -1;
  }

  pushBackNode(list, newNode);

  return 0;
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
