#include "tlo/sllist.h"
#include <assert.h>

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
  tloSLListNode *previous = NULL;
  tloSLListNode *current = list->head;
  while (current) {
    list->type->destruct(current->bytes);
    list->allocator->free(current->bytes);
    if (previous) {
      list->allocator->free(previous);
    }

    previous = current;
    current = current->next;
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
