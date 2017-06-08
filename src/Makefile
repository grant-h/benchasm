NASM=$(PREFIX)nasm
LD=$(PREFIX)gcc
CC=$(PREFIX)gcc

############

SRC=bench.o bench_memcpy.o
ASM_X86=memcpy.o
NASM_FLAGS=


ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        NASM_FLAGS := -f elf
    endif
    ifeq ($(UNAME_S),Darwin)
        NASM_FLAGS := -f macho -d LEADING_UNDERSCORES
    endif
endif

ifeq ($(NASM_FLAGS),)
    $(error "Unsupported OS version")
endif

all : bench_memcpy

bench_memcpy : $(SRC) $(ASM_X86)
	$(LD) -m32 -o $@ $(SRC) $(ASM_X86)

%.o : %.S
	$(NASM) $(NASM_FLAGS) -o $@ $<

%.o : %.s
	$(NASM) $(NASM_FLAGS) -o $@ $<

%.o : %.c
	$(CC) -O2 -m32 -c $< -o $@

clean :
	-rm -f $(ASM_X86) $(SRC) bench_memcpy

.PHONY : clean all
