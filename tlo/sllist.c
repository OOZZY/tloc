#include "tlo/sllist.h"
#include <assert.h>
#include <string.h>

bool tloSLListIsValid(const tloSLList *list) {
  return (
    (list) &&
    (tloTypeIsValid(list->type)) &&
    (tloAllocatorIsValid(list->allocator)) &&
    ((list->head == NULL) == (list->size == 0)) &&
    ((list->tail == NULL) == (list->size == 0)) &&
    ((list->head == NULL) == (list->tail == NULL))
  );
}

int tloSLListConstruct(tloSLList *list, const tloType *type,
                       const tloAllocator *allocator)
{
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

static void freeAllNodes(tloSLList *list) {
  tloSLLNode *previous = NULL;
  tloSLLNode *current = list->head;

  while (current) {
    list->type->destruct(current->bytes);
    list->allocator->free(current->bytes);
    if (previous) {
      list->allocator->free(previous);
    }

    previous = current;
    current = current->next;
  }

  if (previous) {
    list->allocator->free(previous);
  }
}

void tloSLListDestruct(tloSLList *list) {
  if (!list) {
    return;
  }

  assert(tloSLListIsValid(list));

  if (!list->head) {
    return;
  }

  freeAllNodes(list);

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

void tloSLListDelete(tloSLList *list) {
  if (!list) {
    return;
  }

  assert(tloSLListIsValid(list));

  tloSLListDestruct(list);
  tloFreeFunction free = list->allocator->free;
  free(list);
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

void *tloSLListGetFrontRW(tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->head->bytes;
}

const void *tloSLListGetBack(const tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
}

void *tloSLListGetBackRW(tloSLList *list) {
  assert(tloSLListIsValid(list));
  assert(!tloSLListIsEmpty(list));

  return list->tail->bytes;
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

  if (list->type->copyConstruct(node->bytes, data)) {
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

int tloSLListPushBack(tloSLList *list, const void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithCopiedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushBackNode(list, newNode);

  return 0;
}

int tloSLListPushBackMove(tloSLList *list, void *data) {
  assert(tloSLListIsValid(list));
  assert(data);

  tloSLLNode *newNode = makeNodeWithMovedData(list, data);
  if (!newNode) {
    return 1;
  }

  pushBackNode(list, newNode);

  return 0;
}

bool tloSLLNodeIsValid(const tloSLLNode *node) {
  return (
    (node) &&
    (node->bytes)
  );
}

const tloSLLNode *tloSLLNodeGetHead(const tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

tloSLLNode *tloSLLNodeGetHeadRW(tloSLList *list) {
  assert(tloSLListIsValid(list));

  return list->head;
}

const void *tloSLLNodeGetElement(const tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

void *tloSLLNodeGetElementRW(tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->bytes;
}

const tloSLLNode *tloSLLNodeGetNext(const tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}

tloSLLNode *tloSLLNodeGetNextRW(tloSLLNode *node) {
  assert(tloSLLNodeIsValid(node));

  return node->next;
}
