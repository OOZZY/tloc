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
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
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

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntMoveBackOnce(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int value = SOME_NUMBER;
  int error = tloDArrayMoveBack(ints, &value);
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

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntPushBackUntilResize(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &(int){i});
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

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntMoveBackUntilResize(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = (int)i;
    int error = tloDArrayMoveBack(ints, &value);
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

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntPushBackOncePopBackOnce(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  int error = tloDArrayPushBack(ints, &(int){SOME_NUMBER});
  assert(!error);

  tloDArrayPopBack(ints);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) >= tloDArrayGetSize(ints));
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntPushBackUntilResizePopBackUntilEmpty(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (size_t i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &(int){i});
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

  tloDArrayDelete(ints);
  ints = NULL;
}

void testDArrayIntConstructCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *copy = malloc(sizeof(*copy));
  assert(copy);

  int error = tloDArrayConstructCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDestruct(copy);
  free(copy);
  copy = NULL;
}

void testDArrayIntMakeCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *copy = tloDArrayMakeCopy(ints);
  assert(copy);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
}

void testDArrayIntCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < SOME_NUMBER; ++i) {
    int error = tloDArrayPushBack(ints, &i);
    assert(!error);
  }

  tloDArray *copy = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(copy);

  int error = tloDArrayCopy(copy, ints);
  assert(!error);

  assert(tloDArrayGetSize(ints) == tloDArrayGetSize(copy));
  assert(tloDArrayGetCapacity(ints) == tloDArrayGetCapacity(copy));
  assert(tloDArrayIsEmpty(ints) == tloDArrayIsEmpty(copy));
  assert(tloDArrayGetType(ints) == tloDArrayGetType(copy));
  assert(tloDArrayGetAllocator(ints) == tloDArrayGetAllocator(copy));

  for (size_t i = 0; i < tloDArrayGetSize(ints); ++i) {
    const int *elem1 = tloDArrayGetElement(ints, i);
    const int *elem2 = tloDArrayGetElement(copy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  ints = NULL;

  tloDArrayDelete(copy);
  copy = NULL;
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
