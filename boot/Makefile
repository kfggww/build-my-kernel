AS := as
CC := gcc
MAKE := make
OBJECTS := head.o

.PHONY: all clean

all: kernel

clean:
	@echo "开始清零构建结果..."
	@rm $(OBJECTS) kernel
	@echo "清理完毕!"

kernel: head
	@objcopy -S -O binary $< $@

%.o: %.s
	@$(CC) -m32 -ffreestanding -fno-builtin -o $@ $<

head: head.o
	@$(CC) -m elf_i386 -nostartfiles -nolibc -o head head.o
