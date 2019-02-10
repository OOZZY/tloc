#include "schtable_test.h"
#include <stdio.h>
#include <tlo/schtable.h>
#include "map_test_utils.h"
#include "set_test_utils.h"
#include "util.h"

static TloSet *makeSetInt(void) {
  return (TloSet *)tloSCHTableSetMake(&tloInt, &countingAllocator);
}

static TloMap *makeMapIntInt(void) {
  return (TloMap *)tloSCHTableMapMake(&tloInt, &tloInt, &countingAllocator);
}

void testSCHTable(void) {
  testInitialCounts();

  testSetIntInsertOnce(makeSetInt(), true);
  testSetIntInsertOnce(makeSetInt(), false);
  testSetIntInsertManyTimes(makeSetInt(), true);
  testSetIntInsertManyTimes(makeSetInt(), false);

  testMapIntIntInsertOnce(makeMapIntInt(), true);
  testMapIntIntInsertOnce(makeMapIntInt(), false);
  testMapIntIntInsertManyTimes(makeMapIntInt(), true);
  testMapIntIntInsertManyTimes(makeMapIntInt(), false);

  printf("sizeof(TloSCHTableSet): %zu\n", sizeof(TloSCHTableSet));
  printf("sizeof(TloSCHTableMap): %zu\n", sizeof(TloSCHTableMap));
  testFinalCounts();
  puts("====================");
  puts("SCHTable tests done.");
  puts("====================");
}
