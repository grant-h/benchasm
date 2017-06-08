#include <string.h>

#include "bench.h"

extern void * memcpy_mov(void * dst, const void * src, size_t sz);
extern void * memcpy_rep_movsb(void * dst, const void * src, size_t sz);
extern void * memcpy_rep_movsd(void * dst, const void * src, size_t sz);
extern void * memcpy_sse_movntdq(void * dst, const void * src, size_t sz);
extern void * memcpy_sse_movdqa(void * dst, const void * src, size_t sz);
extern void * memcpy_avx2_vmovdqa(void * dst, const void * src, size_t sz);

int main()
{
  size_t runs = 5000;
  size_t size = 1024*1024*1;

  bench_memcpy("memcpy_avx2_vmovntdqa", memcpy_avx2_vmovdqa, size, runs);
  bench_memcpy("memcpy_sse_movntdq", memcpy_sse_movntdq, size, runs);
  bench_memcpy("memcpy_sse_movdqa", memcpy_sse_movdqa, size, runs);
  bench_memcpy("memcpy_rep_movsb", memcpy_rep_movsb, size, runs);

  // movsd seems slower. see - https://stackoverflow.com/questions/8425022/performance-of-x86-rep-instructions-on-modern-pipelined-superscalar-processors#comment57910209_8430285
  bench_memcpy("memcpy_rep_movsd", memcpy_rep_movsd, size, runs);

  // quite slow!
  bench_memcpy("memcpy_mov", memcpy_mov, size, runs);
  bench_memcpy("memcpy", memcpy, size, runs);

  return 0;
}
