# BenchASM
A personal collection of x86 assembly snippets for learning the performance benefits and interesting usages.
Initially spawned from an interest in using SSE/AVX instructions

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
`$ ./bench_memcpy`
