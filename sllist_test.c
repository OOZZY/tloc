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

void testSLListIntPushFrontOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackRW(ints) == SOME_NUMBER);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushFrontMoveOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  int value = SOME_NUMBER;
  error = tloSLListPushFrontMove(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackRW(ints) == SOME_NUMBER);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushFrontManyTimes(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloSLListPushFront(ints, &(int){i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int){i});
    assert(*(int *)tloSLListGetFrontRW(ints) == (int){i});
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetBackRW(ints) == 0);
  }

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushFrontMoveManyTimes(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    error = tloSLListPushFrontMove(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int){i});
    assert(*(int *)tloSLListGetFrontRW(ints) == (int){i});
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetBackRW(ints) == 0);
  }

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushFrontOncePopFrontOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloSLListPushFront(ints, &(int){SOME_NUMBER});
  assert(!error);

  tloSLListPopFront(ints);

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushFrontManyTimesPopFrontUntilEmpty(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloSLListPushFront(ints, &(int){i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == (int){i});
    assert(*(int *)tloSLListGetFrontRW(ints) == (int){i});
    assert(*(const int *)tloSLListGetBack(ints) == 0);
    assert(*(int *)tloSLListGetBackRW(ints) == 0);

    tloSLListPopFront(ints);
  }

  assert(tloSLListGetSize(ints) == 0);
  assert(tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushBackOnce(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloSLListPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloSLListGetSize(ints) == 1);
  assert(!tloSLListIsEmpty(ints));
  assert(tloSLListGetType(ints) == &tloIntType);
  assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackRW(ints) == SOME_NUMBER);

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

  assert(*(const int *)tloSLListGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloSLListGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloSLListGetBackRW(ints) == SOME_NUMBER);

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntPushBackManyTimes(void) {
  tloSLList *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloSLListConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloSLListPushBack(ints, &(int){i});
    assert(!error);

    assert(tloSLListGetSize(ints) == i + 1);
    assert(!tloSLListIsEmpty(ints));
    assert(tloSLListGetType(ints) == &tloIntType);
    assert(tloSLListGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetFrontRW(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int){i});
    assert(*(int *)tloSLListGetBackRW(ints) == (int){i});
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

    assert(*(const int *)tloSLListGetFront(ints) == 0);
    assert(*(int *)tloSLListGetFrontRW(ints) == 0);
    assert(*(const int *)tloSLListGetBack(ints) == (int){i});
    assert(*(int *)tloSLListGetBackRW(ints) == (int){i});
  }

  tloSLListDestruct(ints);
  free(ints);

  ints = NULL;
}

void testSLListIntConstructCopy(void) {
  tloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  tloSLList *intsCopy = malloc(sizeof(*intsCopy));
  assert(intsCopy);

  int error = tloSLListConstructCopy(intsCopy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(intsCopy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(intsCopy));
  assert(tloSLListGetType(ints) == tloSLListGetType(intsCopy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(intsCopy));

  const tloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const tloSLLNode *node2 = tloSLLNodeGetHead(intsCopy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);

  tloSLListDestruct(intsCopy);
  free(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testSLListIntMakeCopy(void) {
  tloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  tloSLList *intsCopy = tloSLListMakeCopy(ints);
  assert(intsCopy);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(intsCopy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(intsCopy));
  assert(tloSLListGetType(ints) == tloSLListGetType(intsCopy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(intsCopy));

  const tloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const tloSLLNode *node2 = tloSLLNodeGetHead(intsCopy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  tloSLListDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testSLListIntCopy(void) {
  tloSLList *ints = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloSLListPushBack(ints, &i);
    assert(!error);
  }

  tloSLList *intsCopy = tloSLListMake(&tloIntType, &tloCountingAllocator);
  assert(intsCopy);

  int error = tloSLListCopy(intsCopy, ints);
  assert(!error);

  assert(tloSLListGetSize(ints) == tloSLListGetSize(intsCopy));
  assert(tloSLListIsEmpty(ints) == tloSLListIsEmpty(intsCopy));
  assert(tloSLListGetType(ints) == tloSLListGetType(intsCopy));
  assert(tloSLListGetAllocator(ints) == tloSLListGetAllocator(intsCopy));

  const tloSLLNode *node1 = tloSLLNodeGetHead(ints);
  const tloSLLNode *node2 = tloSLLNodeGetHead(intsCopy);
  while (node1) {
    assert(node2);
    const int *elem1 = tloSLLNodeGetElement(node1);
    const int *elem2 = tloSLLNodeGetElement(node2);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
    node1 = tloSLLNodeGetNext(node1);
    node2 = tloSLLNodeGetNext(node2);
  }

  tloSLListDelete(ints);
  tloSLListDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

int main(void) {
  printf("sizeof(tloSLList): %zu\n", sizeof(tloSLList));
  printf("sizeof(tloSLLNode): %zu\n", sizeof(tloSLLNode));
  assert(tloCountingAllocatorMallocCount == 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);

  testSLListIntConstructDestruct();
  testSLListIntMakeDelete();
  testSLListDestructWithNull();
  testSLListDeleteWithNull();
  testSLListIntPushFrontOnce();
  testSLListIntPushFrontMoveOnce();
  testSLListIntPushFrontManyTimes();
  testSLListIntPushFrontMoveManyTimes();
  testSLListIntPushFrontOncePopFrontOnce();
  testSLListIntPushFrontManyTimesPopFrontUntilEmpty();
  testSLListIntPushBackOnce();
  testSLListIntPushBackMoveOnce();
  testSLListIntPushBackManyTimes();
  testSLListIntPushBackMoveManyTimes();
  testSLListIntConstructCopy();
  testSLListIntMakeCopy();
  testSLListIntCopy();

  printf("malloc count: %lu; free count: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
}
