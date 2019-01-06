#include "tlo/statistics.h"
#include <assert.h>
#include <math.h>

void tloStatAccConstruct(TloStatAccumulator *accumulator) {
  assert(accumulator);

  accumulator->size = 0;
}

void tloStatAccAdd(TloStatAccumulator *accumulator, long double value) {
  assert(accumulator);

  accumulator->size++;

  if (accumulator->size == 1) {
    accumulator->sum = value;
    accumulator->sumOfSquares = value * value;
    accumulator->minimum = value;
    accumulator->maximum = value;
  } else {
    accumulator->sum += value;
    accumulator->sumOfSquares += value * value;

    if (accumulator->minimum > value) {
      accumulator->minimum = value;
    }

    if (accumulator->maximum < value) {
      accumulator->maximum = value;
    }
  }
}

size_t tloStatAccSize(const TloStatAccumulator *accumulator) {
  assert(accumulator);

  return accumulator->size;
}

long double tloStatAccSum(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  return accumulator->sum;
}

long double tloStatAccMean(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  return accumulator->sum / accumulator->size;
}

long double tloStatAccVariance(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  long double meanOfSquares = accumulator->sumOfSquares / accumulator->size;
  long double mean = tloStatAccMean(accumulator);
  long double squareOfMean = mean * mean;
  return meanOfSquares - squareOfMean;
}

long double tloStatAccStandardDeviation(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  long double variance = tloStatAccVariance(accumulator);
  return sqrtl(variance);
}

long double tloStatAccMinimum(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  return accumulator->minimum;
}

long double tloStatAccMaximum(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);

  return accumulator->maximum;
}

long double tloStatAccRange(const TloStatAccumulator *accumulator) {
  assert(accumulator);
  assert(accumulator->size);
  assert(accumulator->minimum <= accumulator->maximum);

  return accumulator->maximum - accumulator->minimum;
}
