#include <tlo/darray.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define SOME_NUMBER 42

void testDArrayIntConstructDestruct(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntMakeDelete(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);

  ints = NULL;
}

void testDArrayDestructWithNull(void) {
  tloDArrayDestruct(NULL);
}

void testDArrayDeleteWithNull(void) {
  tloDArrayDelete(NULL);
}

void testDArrayIntPushBackOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetElementRW(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetBackRW(ints) == SOME_NUMBER);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackMoveOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  int value = SOME_NUMBER;
  error = tloDArrayPushBackMove(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetElementRW(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetFrontRW(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetBackRW(ints) == SOME_NUMBER);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackOncePopBackOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  tloDArrayPopBack(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackUntilResize(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloDArrayPushBack(ints, &(int){i});
    assert(!error);

    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetElementRW(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetFrontRW(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetBackRW(ints) == (int){i});
  }

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackMoveUntilResize(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    error = tloDArrayPushBackMove(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetElementRW(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetFrontRW(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetBackRW(ints) == (int){i});
  }

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    error = tloDArrayPushBack(ints, &(int){i});
    assert(!error);
  }

  for (size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetElementRW(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetFrontRW(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetBackRW(ints) == (int){i});

    tloDArrayPopBack(ints);
  }

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntCopyConstruct(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *intsCopy = malloc(sizeof(*intsCopy));
  assert(intsCopy);

  int error = tloDArrayCopyConstruct(intsCopy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(intsCopy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(intsCopy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(intsCopy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(intsCopy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(intsCopy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(intsCopy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);

  tloDArrayDestruct(intsCopy);
  free(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testDArrayIntMakeCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *intsCopy = tloDArrayMakeCopy(ints);
  assert(intsCopy);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(intsCopy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(intsCopy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(intsCopy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(intsCopy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(intsCopy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(intsCopy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  tloDArrayDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testDArrayIntCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *intsCopy = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(intsCopy);

  int error = tloDArrayCopy(intsCopy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(intsCopy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(intsCopy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(intsCopy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(intsCopy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(intsCopy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(intsCopy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  tloDArrayDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

int main(void) {
  printf("sizeof(tloDArray): %zu\n", sizeof(tloDArray));
  assert(tloCountingAllocatorMallocCount == 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);

  testDArrayIntConstructDestruct();
  testDArrayIntMakeDelete();
  testDArrayDestructWithNull();
  testDArrayDeleteWithNull();
  testDArrayIntPushBackOnce();
  testDArrayIntPushBackMoveOnce();
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResize();
  testDArrayIntPushBackMoveUntilResize();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntCopyConstruct();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();

  printf("malloc count: %lu; free count: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
}
