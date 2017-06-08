#include "bench.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <assert.h>
#include <sys/time.h>

#include "timersub.h"

struct aligned_ptr {
  void * p;
  void * original;
  uint8_t offset;
};

void bench_init_memcpy(size_t size, struct aligned_ptr * dst, struct aligned_ptr * src);
void bench_fini_memcpy(struct aligned_ptr * dst, struct aligned_ptr * src);
void bench_init_timing(int runs, struct timer ** timers);
void bench_print_stats(const char * name, struct timer * timers, size_t size, int runs);

void bench_init_memcpy(size_t size, struct aligned_ptr * dst, struct aligned_ptr * src)
{
  void * src_original = malloc(size+31);
  void * dst_original = malloc(size+31);

  dst->original = dst_original;
  src->original = src_original;

  dst->p = (void*)(((uintptr_t)dst_original + 31) & ~(uintptr_t)0x1f); // 16-byte alignment
  dst->offset = (uint8_t)((uintptr_t)dst->p - (uintptr_t)dst_original);

  src->p = (void*)(((uintptr_t)src_original + 31) & ~(uintptr_t)0x1f); // 16-byte alignment
  src->offset = (uint8_t)((uintptr_t)src->p - (uintptr_t)src_original);

  if(!(dst_original) || !(src_original)) {
    printf("%s - out of memory\n", __func__);
    assert(0);
  }

#ifdef BENCH_CORRECTNESS
  memset(dst->p, 0, size);
  memset(src->p, 0x41424344, size);
#endif

  return;
}

void bench_fini_memcpy(struct aligned_ptr * dst, struct aligned_ptr * src)
{
  free(dst->original);
  free(src->original);

  dst->original = 0;
  dst->p = 0;
  dst->offset = 0;

  src->original = 0;
  src->p = 0;
  src->offset = 0;
}

void bench_init_timing(int runs, struct timer ** timers)
{
  *timers = malloc(sizeof(struct timer)*runs);

  if(!(*timers)) {
    printf("%s - out of memory\n", __func__);
    assert(0);
  }
}

void bench_print_stats(const char * name, struct timer * timers, size_t size, int runs)
{
  int i;

  float max = 0.0;
  float min = 40000000.0;
  float avg = 0.0;
  float median = 0.0;

  for(i = 0; i < runs; i++)
  {
    float t = timers[i].time;

    if(t > max)
      max = t;

    if(t < min)
      min = t;

    avg += t;
  }

  avg /= runs;

  if(runs % 2 == 0) {
    median = (timers[runs/2].time + timers[runs/2-1].time)/2.0;
  } else {
    median = timers[runs/2].time;
  }

  printf("%s[size %zu, runs %d] - avg %.6f, med %.6f, min %.6f, max %.6f\n\n", name, size, runs,
      avg, median, min, max);
}

void bench_memcpy(const char * name, memcpy_fn function, size_t size, int runs)
{
  int i;
  struct aligned_ptr dst = {0};
  struct aligned_ptr src = {0};
  struct timer * timers = NULL;

  bench_init_memcpy(size, &dst, &src);
  bench_init_timing(runs, &timers);

  fprintf(stderr, "Benchmarking %s...", name);

  int next_print = 0;

  for(i = 0; i < runs; i++) {

    struct timeval start, end, wall_time;

    gettimeofday(&start, NULL);
    {
      function(dst.p, src.p, size);
    }
    gettimeofday(&end, NULL);

#ifdef BENCH_CORRECTNESS
    assert(memcmp(dst.p, src.p, size) == 0);
#endif

    if(next_print == i) {
      int per = i*100/runs;

      if(per % 10 == 0) {
        fprintf(stderr, "%d%%.", per);
      } else if(per % 2 == 0) {
        fprintf(stderr, ".");
      }

      next_print = (per+1)*runs/100;
    }

    timersub(&end, &start, &wall_time);
    timers[i].time = wall_time.tv_sec + (float)wall_time.tv_usec/1000000.0;
  }

  fprintf(stderr, "\n");
  bench_print_stats(name, timers, size, runs);

  free(timers);
  bench_fini_memcpy(&dst, &src);
}
