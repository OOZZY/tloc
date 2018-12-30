#include "tlo/stopwatch.h"
#include <assert.h>

void tloStopwatchStart(TloStopwatch *stopwatch) {
  assert(stopwatch);

  stopwatch->startTime = clock();
}

void tloStopwatchStop(TloStopwatch *stopwatch) {
  assert(stopwatch);

  stopwatch->stopTime = clock();
}

clock_t tloStopwatchNumTicks(const TloStopwatch *stopwatch) {
  assert(stopwatch);
  assert(stopwatch->startTime <= stopwatch->stopTime);

  return stopwatch->stopTime - stopwatch->startTime;
}

double tloStopwatchNumSeconds(const TloStopwatch *stopwatch) {
  assert(stopwatch);
  assert(stopwatch->startTime <= stopwatch->stopTime);

  return (double)(tloStopwatchNumTicks(stopwatch)) / CLOCKS_PER_SEC;
}
