#include <tlo/sllist.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define SOME_NUMBER 42

void testSLListIntConstructDestruct(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntMakeDelete(void) {
  tloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDelete(ints);

  ints = NULL;
}

void testSLListDestructWithNull(void) {
  tloSLListDestruct(NULL);
}

void testSLListDeleteWithNull(void) {
  tloSLListDelete(NULL);
}

void testSLListIntPushBackCopyOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloSLListPushBackCopy(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFrontReadOnly(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontReadWrite(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBackReadOnly(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackReadWrite(ints) == SOME_NUMBER);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushBackMoveOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  int value = SOME_NUMBER;
  error = tloSLListPushBackMove(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFrontReadOnly(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontReadWrite(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBackReadOnly(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackReadWrite(ints) == SOME_NUMBER);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushBackCopyManyTimes(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloSLListPushBackCopy(ints, &(int){i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFrontReadOnly(ints) == 0);
    assert(*(int *)tloSLListGetFrontReadWrite(ints) == 0);
    assert(*(const int *)tloSLListGetBackReadOnly(ints) == (int){i});
    assert(*(int *)tloSLListGetBackReadWrite(ints) == (int){i});
  }

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushBackMoveManyTimes(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    error = tloSLListPushBackMove(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFrontReadOnly(ints) == 0);
    assert(*(int *)tloSLListGetFrontReadWrite(ints) == 0);
    assert(*(const int *)tloSLListGetBackReadOnly(ints) == (int){i});
    assert(*(int *)tloSLListGetBackReadWrite(ints) == (int){i});
  }

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

int main(void) {
  printf("sizeof(tloSLList): %zu\n", sizeof(tloSLList));
  assert(tloCountingAllocatorMallocCount == 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);

  testSLListIntConstructDestruct();
  testSLListIntMakeDelete();
  testSLListDestructWithNull();
  testSLListDeleteWithNull();
  testSLListIntPushBackCopyOnce();
  testSLListIntPushBackMoveOnce();
  testSLListIntPushBackCopyManyTimes();
  testSLListIntPushBackMoveManyTimes();

  printf("malloc count: %lu; free count: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
}
