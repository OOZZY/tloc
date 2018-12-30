#ifndef TLO_STOPWATCH_H
#define TLO_STOPWATCH_H

#include <time.h>

typedef struct TloStopwatch {
  // private
  clock_t startTime;
  clock_t stopTime;
} TloStopwatch;

// sets start time
void tloStopwatchStart(TloStopwatch *stopwatch);

// sets stop time
void tloStopwatchStop(TloStopwatch *stopwatch);

// returns number of clock ticks between start and stop times
clock_t tloStopwatchNumTicks(const TloStopwatch *stopwatch);

// returns number of seconds between start and stop times
double tloStopwatchNumSeconds(const TloStopwatch *stopwatch);

#endif  // TLO_STOPWATCH_H
