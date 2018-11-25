#include "tlo/list.h"
#include <assert.h>

bool tloListVTableIsValid(const TloListVTable *vTable) {
  return vTable && vTable->isValid && vTable->destruct && vTable->size &&
         vTable->isEmpty && vTable->front && vTable->mutableFront &&
         vTable->back && vTable->mutableBack && vTable->pushBack &&
         vTable->moveBack;
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
