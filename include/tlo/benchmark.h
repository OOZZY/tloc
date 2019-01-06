#ifndef TLO_BENCHMARK_H
#define TLO_BENCHMARK_H

typedef void (*TloBenchmarkTask)(const void *parameters);

void tloTimeTask(TloBenchmarkTask task, const void *parameters,
                 int numIterations, const char *description);

#define TLO_TIME_TASK(_task, _parameters, _numIterations) \
  tloTimeTask(_task, _parameters, _numIterations, #_task)

#endif  // TLO_BENCHMARK_H
