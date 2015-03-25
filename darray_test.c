#include <tlo/darray.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define SOME_NUMBER 42

void testDArrayIntConstructDestructStackSpace(void) {
  tloDArray ints;

  int error = tloDArrayConstruct(&ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) >= tloDArrayGetSize(&ints));
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetType(&ints) == &tloIntType);
  assert(tloDArrayGetAllocator(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

void testDArrayIntConstructDestructHeapSpace(void) {
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

void testDArrayIntConstructWithCapacityDestructStackSpace(void) {
  tloDArray ints;

  int error =
    tloDArrayConstructWithCapacity(
      &ints, &tloIntType, &tloCountingAllocator, SOME_NUMBER
    );
  assert(!error);

  assert(tloDArrayGetSize(&ints) == 0);
  assert(tloDArrayGetCapacity(&ints) == SOME_NUMBER);
  assert(tloDArrayIsEmpty(&ints));
  assert(tloDArrayGetType(&ints) == &tloIntType);
  assert(tloDArrayGetAllocator(&ints) == &tloCountingAllocator);

  tloDArrayDestruct(&ints);
}

void testDArrayIntConstructWithCapacityDestructHeapSpace(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error =
    tloDArrayConstructWithCapacity(
      ints, &tloIntType, &tloCountingAllocator, SOME_NUMBER
    );
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
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

void testDArrayIntMakeWithCapacityDelete(void) {
  tloDArray *ints =
    tloDArrayMakeWithCapacity(&tloIntType, &tloCountingAllocator, SOME_NUMBER);
  assert(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == SOME_NUMBER);
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
  assert(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntMoveBackOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  int value = SOME_NUMBER;
  error = tloDArrayMoveBack(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElement(ints, 0) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableElement(ints, 0) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetFront(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableFront(ints) == SOME_NUMBER);
  assert(*(const int *)tloDArrayGetBack(ints) == SOME_NUMBER);
  assert(*(int *)tloDArrayGetMutableBack(ints) == SOME_NUMBER);

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
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int){i});
  }

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntMoveBackUntilResize(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    error = tloDArrayMoveBack(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElement(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int){i});
  }

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
    assert(*(int *)tloDArrayGetMutableElement(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFront(ints) == 0);
    assert(*(int *)tloDArrayGetMutableFront(ints) == 0);
    assert(*(const int *)tloDArrayGetBack(ints) == (int){i});
    assert(*(int *)tloDArrayGetMutableBack(ints) == (int){i});

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

void testDArrayIntConstructCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *intsCopy = malloc(sizeof(*intsCopy));
  assert(intsCopy);

  int error = tloDArrayConstructCopy(intsCopy, ints);
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
  assert(tloCountingAllocatorTotalByteCount == 0);

  testDArrayIntConstructDestructStackSpace();
  testDArrayIntConstructDestructHeapSpace();
  testDArrayIntConstructWithCapacityDestructStackSpace();
  testDArrayIntConstructWithCapacityDestructHeapSpace();
  testDArrayIntMakeDelete();
  testDArrayIntMakeWithCapacityDelete();
  testDArrayDestructWithNull();
  testDArrayDeleteWithNull();
  testDArrayIntPushBackOnce();
  testDArrayIntMoveBackOnce();
  testDArrayIntPushBackUntilResize();
  testDArrayIntMoveBackUntilResize();
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntConstructCopy();
  testDArrayIntMakeCopy();
  testDArrayIntCopy();

  printf("malloc count: %lu\nfree count: %lu\ntotal bytes allocated: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount,
         tloCountingAllocatorTotalByteCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorTotalByteCount > 0);
}
