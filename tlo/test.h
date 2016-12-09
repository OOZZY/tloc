#ifndef TLO_TEST_H
#define TLO_TEST_H

#include <stdbool.h>

void tloTestExpect(bool isExpect, bool condition, const char *file, int line,
                   const char *func, const char *conditionString);

#define TLO_EXPECT(condition) \
  tloTestExpect(true, (condition), __FILE__, __LINE__, __func__, #condition)

#define TLO_ASSERT(condition)                                       \
  do {                                                              \
    tloTestExpect(false, (condition), __FILE__, __LINE__, __func__, \
                  #condition);                                      \
    if (!(condition)) {                                             \
      return;                                                       \
    }                                                               \
  } while (0)

void tloTestPrintReport(void);
void tloTestExit(void);

#endif  // TLO_TEST_H
