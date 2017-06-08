#ifndef _BENCH_H
#define _BENCH_H

#include <stddef.h>

struct timer {
  float time;
};

typedef void * (*memcpy_fn)(void* dst, const void* src, size_t sz);

void bench_memcpy(const char * name, memcpy_fn function, size_t size, int runs);

#endif
