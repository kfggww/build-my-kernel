

ASM := nasm

.PHONY: all clean

all: kernel.bin

clean:
	rm kernel.bin

kernel.bin: kernel_init.asm
	$(ASM) -f bin -o $@ $<
