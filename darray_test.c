#include <tlo/darray.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void testDArrayIntConstructDestruct(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  assert(tloDArrayGetSize(ints) == 0);
  assert(tloDArrayGetCapacity(ints) == 0);
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
  assert(tloDArrayGetCapacity(ints) == 0);
  assert(tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  tloDArrayDelete(ints);

  ints = NULL;
}

void testDArrayIntPushBackCopyOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  error = tloDArrayPushBackCopy(ints, &(int){42});
  assert(!error);

  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= 1);
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElementReadOnly(ints, 0) == 42);
  assert(*(int *)tloDArrayGetElementReadWrite(ints, 0) == 42);
  assert(*(const int *)tloDArrayGetFrontReadOnly(ints) == 42);
  assert(*(int *)tloDArrayGetFrontReadWrite(ints) == 42);
  assert(*(const int *)tloDArrayGetBackReadOnly(ints) == 42);
  assert(*(int *)tloDArrayGetBackReadWrite(ints) == 42);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackMoveOnce(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  int value = 42;
  error = tloDArrayPushBackMove(ints, &value);
  assert(!error);

  assert(value == 0);
  assert(tloDArrayGetSize(ints) == 1);
  assert(tloDArrayGetCapacity(ints) >= 1);
  assert(!tloDArrayIsEmpty(ints));
  assert(tloDArrayGetType(ints) == &tloIntType);
  assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

  assert(*(const int *)tloDArrayGetElementReadOnly(ints, 0) == 42);
  assert(*(int *)tloDArrayGetElementReadWrite(ints, 0) == 42);
  assert(*(const int *)tloDArrayGetFrontReadOnly(ints) == 42);
  assert(*(int *)tloDArrayGetFrontReadWrite(ints) == 42);
  assert(*(const int *)tloDArrayGetBackReadOnly(ints) == 42);
  assert(*(int *)tloDArrayGetBackReadWrite(ints) == 42);

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntPushBackCopyUntilResize(void) {
  tloDArray *ints = malloc(sizeof(*ints));
  assert(ints);

  int error = tloDArrayConstruct(ints, &tloIntType, &tloCountingAllocator);
  assert(!error);

  for (size_t i = 0; i < 42; ++i) {
    error = tloDArrayPushBackCopy(ints, &(int){i});
    assert(!error);

    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= i + 1);
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElementReadOnly(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetElementReadWrite(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFrontReadOnly(ints) == 0);
    assert(*(int *)tloDArrayGetFrontReadWrite(ints) == 0);
    assert(*(const int *)tloDArrayGetBackReadOnly(ints) == (int){i});
    assert(*(int *)tloDArrayGetBackReadWrite(ints) == (int){i});
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

  for (size_t i = 0; i < 42; ++i) {
    int value = (int)i;
    error = tloDArrayPushBackMove(ints, &value);
    assert(!error);

    assert(value == 0);
    assert(tloDArrayGetSize(ints) == i + 1);
    assert(tloDArrayGetCapacity(ints) >= i + 1);
    assert(!tloDArrayIsEmpty(ints));
    assert(tloDArrayGetType(ints) == &tloIntType);
    assert(tloDArrayGetAllocator(ints) == &tloCountingAllocator);

    assert(*(const int *)tloDArrayGetElementReadOnly(ints, i) == (int){i});
    assert(*(int *)tloDArrayGetElementReadWrite(ints, i) == (int){i});
    assert(*(const int *)tloDArrayGetFrontReadOnly(ints) == 0);
    assert(*(int *)tloDArrayGetFrontReadWrite(ints) == 0);
    assert(*(const int *)tloDArrayGetBackReadOnly(ints) == (int){i});
    assert(*(int *)tloDArrayGetBackReadWrite(ints) == (int){i});
  }

  tloDArrayDestruct(ints);
  free(ints);

  ints = NULL;
}

void testDArrayIntCopy(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < 42; ++i) {
    int error = tloDArrayPushBackCopy(ints, &i);
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
    const int *elem1 = tloDArrayGetElementReadOnly(ints, i);
    const int *elem2 = tloDArrayGetElementReadOnly(intsCopy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  tloDArrayDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testDArrayIntCopyConstruct(void) {
  tloDArray *ints = tloDArrayMake(&tloIntType, &tloCountingAllocator);
  assert(ints);

  for (int i = 0; i < 42; ++i) {
    int error = tloDArrayPushBackCopy(ints, &i);
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
    const int *elem1 = tloDArrayGetElementReadOnly(ints, i);
    const int *elem2 = tloDArrayGetElementReadOnly(intsCopy, i);
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

  for (int i = 0; i < 42; ++i) {
    int error = tloDArrayPushBackCopy(ints, &i);
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
    const int *elem1 = tloDArrayGetElementReadOnly(ints, i);
    const int *elem2 = tloDArrayGetElementReadOnly(intsCopy, i);
    assert(elem1 != elem2);
    assert(*elem1 == *elem2);
  }

  tloDArrayDelete(ints);
  tloDArrayDelete(intsCopy);

  ints = NULL;
  intsCopy = NULL;
}

void testDArrayDestructWithNull(void) {
  tloDArrayDestruct(NULL);
}

void testDArrayDeleteWithNull(void) {
  tloDArrayDelete(NULL);
}

int main(void) {
  printf("sizeof(tloDArray): %zu\n", sizeof(tloDArray));
  assert(tloCountingAllocatorMallocCount == 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);

  testDArrayIntConstructDestruct();
  testDArrayIntMakeDelete();
  testDArrayIntPushBackCopyOnce();
  testDArrayIntPushBackMoveOnce();
  testDArrayIntPushBackCopyUntilResize();
  testDArrayIntPushBackMoveUntilResize();
  testDArrayIntCopy();
  testDArrayIntCopyConstruct();
  testDArrayIntMakeCopy();
  testDArrayDestructWithNull();
  testDArrayDeleteWithNull();

  printf("malloc count: %lu; free count: %lu\n",
         tloCountingAllocatorMallocCount,
         tloCountingAllocatorFreeCount);
  assert(tloCountingAllocatorMallocCount > 0);
  assert(tloCountingAllocatorMallocCount == tloCountingAllocatorFreeCount);
}
