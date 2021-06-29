AS := as
CC := gcc
MAKE := make
LD = ld
OBJECTS := head.o head

.PHONY: all clean

all: kernel

clean:
	@echo "开始清零构建结果..."
	@rm $(OBJECTS) kernel
	@echo "清理完毕!"

kernel: head
	@objcopy -O binary -j .text $< $@

%.o: %.s
	@$(AS) --32 -o $@ $<

head: head.o
	@$(LD) -m elf_i386 -o head head.o
