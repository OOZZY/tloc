#include "tlo/list.h"
#include <assert.h>

bool tloListVTableIsValid(const TloListVTable *vTable) {
  return vTable && vTable->type && vTable->isValid && vTable->destruct &&
         vTable->size && vTable->isEmpty && vTable->front &&
         vTable->mutableFront && vTable->back && vTable->mutableBack &&
         vTable->pushBack && vTable->moveBack;
}

bool tloListIsValid(const TloList *list) {
  return list && tloListVTableIsValid(list->vTable) &&
         tloTypeIsValid(list->valueType) &&
         tloAllocatorTypeIsValid(list->allocatorType);
}

void tloListConstruct(TloList *list, const TloListVTable *vTable,
                      const TloType *valueType,
                      const TloAllocatorType *allocatorType) {
  assert(list);
  assert(vTable);
  assert(tloTypeIsValid(valueType));

  if (!allocatorType) {
    allocatorType = &tloCStdLibAllocator;
  }

  assert(tloAllocatorTypeIsValid(allocatorType));

  list->vTable = vTable;
  list->valueType = valueType;
  list->allocatorType = allocatorType;
}

void tloListDelete(TloList *list) {
  if (!list) {
    return;
  }

  assert(tlovListIsValid(list));

  tlovListDestruct(list);
  list->allocatorType->free(list);
}

const TloType *tloListValueType(const TloList *list) {
  assert(tlovListIsValid(list));

  return list->valueType;
}

const TloAllocatorType *tloListAllocatorType(const TloList *list) {
  assert(tlovListIsValid(list));

  return list->allocatorType;
}

static unsigned char getFunctions(const TloListVTable *vTable) {
  unsigned char functions = 0;

  if (vTable->capacity) {
    functions |= TLO_LIST_CAPACITY;
  }

  if (vTable->element && vTable->mutableElement) {
    functions |= TLO_LIST_ELEMENT;
  }

  if (vTable->pushFront && vTable->moveFront) {
    functions |= TLO_LIST_PUSH_FRONT;
  }

  if (vTable->popFront) {
    functions |= TLO_LIST_POP_FRONT;
  }

  if (vTable->popBack) {
    functions |= TLO_LIST_POP_BACK;
  }

  if (vTable->unorderedRemove) {
    functions |= TLO_LIST_UNORDERED_REMOVE;
  }

  return functions;
}

bool tloListHasFunctions(const TloList *list, unsigned char functions) {
  assert(tloListIsValid(list));

  return (getFunctions(list->vTable) & functions) == functions;
}

const char *tlovListType(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->type;
}

bool tlovListIsValid(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->isValid(list);
}

void tlovListDestruct(TloList *list) {
  assert(tloListIsValid(list));

  list->vTable->destruct(list);
}

size_t tlovListSize(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->size(list);
}

bool tlovListIsEmpty(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->isEmpty(list);
}

const void *tlovListFront(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->front(list);
}

void *tlovListMutableFront(TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->mutableFront(list);
}

const void *tlovListBack(const TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->back(list);
}

void *tlovListMutableBack(TloList *list) {
  assert(tloListIsValid(list));

  return list->vTable->mutableBack(list);
}

TloError tlovListPushBack(TloList *list, const void *data) {
  assert(tloListIsValid(list));

  return list->vTable->pushBack(list, data);
}

TloError tlovListMoveBack(TloList *list, void *data) {
  assert(tloListIsValid(list));

  return list->vTable->moveBack(list, data);
}

size_t tlovListCapacity(const TloList *list) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_CAPACITY));

  return list->vTable->capacity(list);
}

const void *tlovListElement(const TloList *list, size_t index) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_ELEMENT));

  return list->vTable->element(list, index);
}

void *tlovListMutableElement(TloList *list, size_t index) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_ELEMENT));

  return list->vTable->mutableElement(list, index);
}

TloError tlovListPushFront(TloList *list, const void *data) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_PUSH_FRONT));

  return list->vTable->pushFront(list, data);
}

TloError tlovListMoveFront(TloList *list, void *data) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_PUSH_FRONT));

  return list->vTable->moveFront(list, data);
}

void tlovListPopFront(TloList *list) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_POP_FRONT));

  list->vTable->popFront(list);
}

void tlovListPopBack(TloList *list) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_POP_BACK));

  list->vTable->popBack(list);
}

void tlovListUnorderedRemove(TloList *list, size_t index) {
  assert(tloListIsValid(list));
  assert(tloListHasFunctions(list, TLO_LIST_UNORDERED_REMOVE));

  list->vTable->unorderedRemove(list, index);
}
