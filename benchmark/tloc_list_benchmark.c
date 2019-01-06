#include <stdio.h>
#include <stdlib.h>
#include <tlo/benchmark.h>
#include <tlo/cdarray.h>
#include <tlo/darray.h>
#include <tlo/dllist.h>
#include <tlo/list.h>
#include <tlo/sllist.h>

static void pushBackThenPopBack(TloList *list, size_t maxListSize) {
  for (size_t i = 0; i < maxListSize; ++i) {
    tlovListPushBack(list, &i);
  }

  while (!tlovListIsEmpty(list)) {
    tlovListPopBack(list);
  }

  tloListDelete(list);
}

static void darrayPushBackThenPopBack(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopBack((TloList *)tloDArrayMake(&tloInt, NULL, 0), *maxListSize);
}

static void cdarrayPushBackThenPopBack(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopBack((TloList *)tloCDArrayMake(&tloInt, NULL, 0),
                      *maxListSize);
}

static void dllistPushBackThenPopBack(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopBack((TloList *)tloDLListMake(&tloInt, NULL), *maxListSize);
}

static void pushFrontThenPopFront(TloList *list, size_t maxListSize) {
  for (size_t i = 0; i < maxListSize; ++i) {
    tlovListPushFront(list, &i);
  }

  while (!tlovListIsEmpty(list)) {
    tlovListPopFront(list);
  }

  tloListDelete(list);
}

static void cdarrayPushFrontThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushFrontThenPopFront((TloList *)tloCDArrayMake(&tloInt, NULL, 0),
                        *maxListSize);
}

static void sllistPushFrontThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushFrontThenPopFront((TloList *)tloSLListMake(&tloInt, NULL), *maxListSize);
}

static void dllistPushFrontThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushFrontThenPopFront((TloList *)tloDLListMake(&tloInt, NULL), *maxListSize);
}

static void pushBackThenPopFront(TloList *list, size_t maxListSize) {
  for (size_t i = 0; i < maxListSize; ++i) {
    tlovListPushBack(list, &i);
  }

  while (!tlovListIsEmpty(list)) {
    tlovListPopFront(list);
  }

  tloListDelete(list);
}

static void cdarrayPushBackThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopFront((TloList *)tloCDArrayMake(&tloInt, NULL, 0),
                       *maxListSize);
}

static void sllistPushBackThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopFront((TloList *)tloSLListMake(&tloInt, NULL), *maxListSize);
}

static void dllistPushBackThenPopFront(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushBackThenPopFront((TloList *)tloDLListMake(&tloInt, NULL), *maxListSize);
}

static void pushFrontThenPopBack(TloList *list, size_t maxListSize) {
  for (size_t i = 0; i < maxListSize; ++i) {
    tlovListPushFront(list, &i);
  }

  while (!tlovListIsEmpty(list)) {
    tlovListPopBack(list);
  }

  tloListDelete(list);
}

static void cdarrayPushFrontThenPopBack(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushFrontThenPopBack((TloList *)tloCDArrayMake(&tloInt, NULL, 0),
                       *maxListSize);
}

static void dllistPushFrontThenPopBack(const void *parameters) {
  const size_t *maxListSize = parameters;
  pushFrontThenPopBack((TloList *)tloDLListMake(&tloInt, NULL), *maxListSize);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s <max-list-size> <num-iterations>\n", argv[0]);
    return 1;
  }

  size_t maxListSize = strtoull(argv[1], NULL, 10);
  if (maxListSize < 1) {
    puts("error: given size is invalid");
    return 1;
  }

  int numIterations = atoi(argv[2]);
  if (numIterations < 1) {
    puts("error: given number of iterations is invalid");
    return 1;
  }

  TLO_TIME_TASK(darrayPushBackThenPopBack, &maxListSize, numIterations);

  TLO_TIME_TASK(cdarrayPushBackThenPopBack, &maxListSize, numIterations);
  TLO_TIME_TASK(cdarrayPushFrontThenPopFront, &maxListSize, numIterations);
  TLO_TIME_TASK(cdarrayPushBackThenPopFront, &maxListSize, numIterations);
  TLO_TIME_TASK(cdarrayPushFrontThenPopBack, &maxListSize, numIterations);

  TLO_TIME_TASK(sllistPushFrontThenPopFront, &maxListSize, numIterations);
  TLO_TIME_TASK(sllistPushBackThenPopFront, &maxListSize, numIterations);

  TLO_TIME_TASK(dllistPushBackThenPopBack, &maxListSize, numIterations);
  TLO_TIME_TASK(dllistPushFrontThenPopFront, &maxListSize, numIterations);
  TLO_TIME_TASK(dllistPushBackThenPopFront, &maxListSize, numIterations);
  TLO_TIME_TASK(dllistPushFrontThenPopBack, &maxListSize, numIterations);
}
