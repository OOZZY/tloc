#ifndef TLO_BENCHMARK_H
#define TLO_BENCHMARK_H

typedef void (*TloBenchmarkTask)(const void *parameters);

void tloBenchmark(TloBenchmarkTask task, const void *parameters,
                  int numIterations, const char *description);

#define TLO_BENCHMARK(_task, _parameters, _numIterations) \
  tloBenchmark(_task, _parameters, _numIterations, #_task)

#endif  // TLO_BENCHMARK_H
