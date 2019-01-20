#ifndef TLO_SCHTABLE_H
#define TLO_SCHTABLE_H

#include "tlo/map.h"
#include "tlo/set.h"

typedef struct TloSCHTNode {
  // private
  unsigned char *data;
  struct TloSCHTNode *next;
} TloSCHTNode;

typedef struct TloSCHTable {
  // private
  TloSCHTNode **array;
  size_t size;
  size_t capacity;
} TloSCHTable;

typedef struct TloSCHTableSet {
  // public, use only for passing to tloSet and tlovSet functions
  TloSet set;

  // private
  TloSCHTable table;
} TloSCHTableSet;

typedef struct TloSCHTableMap {
  // public, use only for passing to tloMap and tlovMap functions
  TloMap map;

  // private
  TloSCHTable table;
} TloSCHTableMap;

void tloSCHTableSetConstruct(TloSCHTableSet *table, const TloType *valueType,
                             const TloAllocator *allocator);
void tloSCHTableMapConstruct(TloSCHTableMap *table, const TloType *keyType,
                             const TloType *valueType,
                             const TloAllocator *allocator);

#endif  // TLO_SCHTABLE_H
