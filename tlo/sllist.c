#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

static void destructAllElementsAndFreeAllNodes(tloSLList *list) {
  tloSLLNode *current = list->head;

  while (current) {
    tloSLLNode *next = current->next;

    list->type->destruct(current->bytes);
    list->allocator->free(current->bytes);
    list->allocator->free(current);

    current = next;
  }
}

static tloSLLNode *makeNodeWithCopiedData(tloSLList *list, const void *data) {
  tloSLLNode *node = list->allocator->malloc(sizeof(*node));
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

static tloSLLNode *makeNodeWithMovedData(tloSLList *list, void *data) {
  tloSLLNode *node = list->allocator->malloc(sizeof(*node));
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

static void pushFrontNode(tloSLList *list, tloSLLNode *node) {
  if (!list->head) {
    list->head = node;
    list->tail = list->head;
  } else {
    node->next = list->head;
    list->head = node;
  }

  ++list->size;
}

static void pushBackNode(tloSLList *list, tloSLLNode *node) {
  if (!list->head) {
    list->head = node;
    list->tail = list->head;
  } else {
    list->tail->next = node;
    list->tail = list->tail->next;
  }

  ++list->size;
}

static tloError pushBackAllElementsOfOther(tloSLList *list,
                                           const tloSLList *other) {
  for (tloSLLNode *node = other->head; node; node = node->next) {
    const void *element = node->bytes;
    if (tloSLListPushBack(list, element)) {
      tloSLListDestruct(list);
      return 1;
    }
  }

  return 0;
}

bool tloSLListIsValid(const tloSLList *list) {
  return list && tloTypeIsValid(list->type) &&
         tloAllocatorIsValid(list->allocator) &&
         ((list->head == NULL) == (list->size == 0)) &&
         ((list->tail == NULL) == (list->size == 0)) &&
         ((list->head == NULL) == (list->tail == NULL)) &&
         ((list->size == 0 || list->size == 1) ? (list->head == list->tail)
                                               : (list->head != list->tail));
}

tloError tloSLListConstruct(tloSLList *list, const tloType *type,
                            const tloAllocator *allocator) {
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

tloError tloSLListConstructCopy(tloSLList *list, const tloSLList *other) {
  assert(list);
  assert(tloSLListIsValid(other));

  if (tloSLListConstruct(list, other->type, other->allocator)) {
    return 1;
  }

  if (pushBackAllElementsOfOther(list, other)) {
    return 1;
  }

  return 0;
}

void tloSLListDestruct(tloSLList *list) {
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

tloSLList *tloSLListMake(const tloType *type, const tloAllocator *allocator) {
  assert(tloTypeIsValid(type));
  assert(tloAllocatorIsValid(allocator));

  tloSLList *list = allocator->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstruct(list, type, allocator)) {
    allocator->free(list);
    return NULL;
  }

  return list;
}

tloSLList *tloSLListMakeCopy(const tloSLList *other) {
  assert(tloSLListIsValid(other));

  tloSLList *list = other->allocator->malloc(sizeof(*list));
  if (!list) {
    return NULL;
  }

  if (tloSLListConstructCopy(list, other)) {
    other->allocator->free(list);
    return NULL;
  }

  return list;
}

void tloSLListDelete(tloSLList *list) {
  if (!list) {
    return;
  }

  assert(tloSLListIsValid(list));

  tloSLListDestruct(list);
  tloFreeFunction free = list->allocator->free;
  free(list);
}

tloError tloSLListCopy(tloSLList *list, const tloSLList *other) {
  assert(tloSLListIsValid(list));
  assert(tloSLListIsValid(other));

  tloSLList copy;
  if (tloSLListConstructCopy(&copy, other)) {
    return 1;
  }

  tloSLListDestruct(list);
  memcpy(list, &copy, sizeof(tloSLList));

  return 0;
}

const tloType *tloSLListGetType(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->type;
}

const tloAllocator *tloSLListGetAllocator(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->allocator;
}

size_t tloSLListGetSize(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->size;
}

bool tloSLListIsEmpty(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->size == 0;
}

const void *tloSLListGetFront(const tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->head->bytes;
}

void *tloSLListGetMutableFront(tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->head->bytes;
}

const void *tloSLListGetBack(const tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
}

void *tloSLListGetMutableBack(tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
}

tloError tloSLListPushFront(tloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushFrontNode(list, newNode);

  return 0;
}

tloError tloSLListMoveFront(tloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushFrontNode(list, newNode);

  return 0;
}

void tloSLListPopFront(tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  tloSLLNode *frontNode = list->head;
  list->head = list->head->next;
  list->type->destruct(frontNode->bytes);
  list->allocator->free(frontNode->bytes);
  list->allocator->free(frontNode);
  --list->size;

  if (!list->head) {
    list->tail = NULL;
  }
}

tloError tloSLListPushBack(tloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushBackNode(list, newNode);

  return 0;
}

tloError tloSLListMoveBack(tloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushBackNode(list, newNode);

  return 0;
}

bool tloSLLNodeIsValid(const tloSLLNode *node) { return node && node->bytes; }

const tloSLLNode *tloSLLNodeGetHead(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

tloSLLNode *tloSLLNodeGetMutableHead(tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

const void *tloSLLNodeGetElement(const tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

void *tloSLLNodeGetMutableElement(tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

const tloSLLNode *tloSLLNodeGetNext(const tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}

tloSLLNode *tloSLLNodeGetMutableNext(tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}
