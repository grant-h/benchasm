# BenchASM
A personal collection of x86 assembly snippets for learning the performance benefits and interesting usages.
Initially spawned from an interest in using SSE/AVX instructions. `gettimeofday` is used for time measurement, but a more accurate counter would be great (i.e `RDTSC`).

## Requirements
* NASM >= 2.13.01 (due to newer AVX instruction mnemonics)
* GCC / Clang with 32-bit support

## Building
```
$ make
gcc -O2 -m32 -c bench.c -o bench.o
gcc -O2 -m32 -c bench_memcpy.c -o bench_memcpy.o
nasm -f macho -o memcpy.o memcpy.a
gcc -m32 -o bench_memcpy bench.o bench_memcpy.o memcpy.o
```

## Running
```
$ ./bench_memcpy # runs different memcpy implementations for 50K runs on 8MiB of data
Benchmarking memcpy_avx2_vmovntdqa...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_avx2_vmovntdqa[size 8388608, runs 50000] - avg 0.001307, med 0.000913, min 0.000800, max 0.084190

Benchmarking memcpy_sse_movntdq...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_sse_movntdq[size 8388608, runs 50000] - avg 0.001532, med 0.001935, min 0.000819, max 0.087775

Benchmarking memcpy_sse_movdqa...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_sse_movdqa[size 8388608, runs 50000] - avg 0.002097, med 0.001780, min 0.001370, max 0.079080

Benchmarking memcpy_rep_movsb...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_rep_movsb[size 8388608, runs 50000] - avg 0.001692, med 0.001668, min 0.001052, max 0.051148

Benchmarking memcpy_rep_movsd...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_rep_movsd[size 8388608, runs 50000] - avg 0.001715, med 0.001967, min 0.001041, max 0.043809

Benchmarking memcpy_mov...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy_mov[size 8388608, runs 50000] - avg 0.004724, med 0.006823, min 0.003722, max 0.104599

Benchmarking memcpy...0%.....10%.....20%.....30%.....40%.....50%.....60%.....70%.....80%.....90%.....
memcpy[size 8388608, runs 50000] - avg 0.002116, med 0.001725, min 0.001372, max 0.126249
```

As we can see the fastest implementation overall is the AVX2 version, but the SSE `MOVNTDQ` instruction does just fine.
It's faster due to usage of non-temporal writes, which are a hint to the memory subsystem that this data will not be used any time soon.
It's unclear how realistic this is and we are hitting up against the core issues with microbenchmarks.
