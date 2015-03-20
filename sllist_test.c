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

int main(void) {
  printf("sizeof(tloSLList): %zu\n", sizeof(tloSLList));
  assert(tloCountingAllocatorMallocCount == 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);

  testSLListIntConstructDestruct();
  testSLListIntMakeDelete();
  testSLListIntPushBackCopyOnce();
  testSLListIntPushBackMoveOnce();

  printf("malloc count: %lu; free count: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
}
