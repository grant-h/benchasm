NASM=$(PREFIX)nasm
LD=$(PREFIX)gcc
CC=$(PREFIX)gcc

############

SRC=bench.o bench_memcpy.o
ASM_X86=memcpy.o
FORMAT=

ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        FORMAT := elf
    endif
    ifeq ($(UNAME_S),Darwin)
        FORMAT := macho
    endif
endif

ifeq ($(FORMAT),)
    $(error "Unsupported OS version")
endif

$(info $(CCFLAGS))

all : bench_memcpy

bench_memcpy : $(SRC) $(ASM_X86)
	$(LD) -m32 -o $@ $(SRC) $(ASM_X86)

%.o : %.a
	$(NASM) -f $(FORMAT) -o $@ $<

%.o : %.c
	$(CC) -O2 -m32 -c $< -o $@

clean :
	-rm -f $(ASM_X86) $(SRC) bench_memcpy

.PHONY : clean all
