#ifndef TLO_STATISTICS_H
#define TLO_STATISTICS_H

#include <stddef.h>

typedef struct TloStatAccumulator {
  // private
  size_t size;
  long double sum;
  long double sumOfSquares;
  long double minimum;
  long double maximum;
} TloStatAccumulator;

void tloStatAccConstruct(TloStatAccumulator *accumulator);
void tloStatAccAdd(TloStatAccumulator *accumulator, long double value);
size_t tloStatAccSize(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccSum(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccMean(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccVariance(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccStandardDeviation(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccMinimum(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccMaximum(const TloStatAccumulator *accumulator);

// assumes tloStatAccSize(accumulator) > 0
long double tloStatAccRange(const TloStatAccumulator *accumulator);

#endif  // TLO_STATISTICS_H
