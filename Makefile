# TINIX

AS = as
CC = gcc
LD = ld

HD_IMG := hd10m.img
OBJECTS := boot/*.o $(HD_IMG) boot/bootsect boot/setup tools/build init/*.o system

# 参考的linux2.4.1, TODO: -f开头的选项和优化有关, strict-aliasing含义有待研究.
# CFLAGS := -Wall -fomit-frame-pointer -fno-strict-aliasing
# 因为汇编得到的是32位elf, 所以链接时也要输出对应的elf文件格式
LDFLAGS := -m elf_i386

.PHONY: all clean

all: Image

Image: tools/build $(HD_IMG) boot/bootsect boot/setup system
	@tools/build boot/bootsect boot/setup system $(HD_IMG)


# 参考linux 2.4.1的Makefile
boot/bootsect.o: boot/bootsect.s
	@$(AS) -o $@ $<

boot/bootsect: boot/bootsect.o
	@$(LD) -Ttext 0x0 -s -oformat=binary -o boot/bootsect.tmp.o $<
	@objcopy -O binary -j .text boot/bootsect.tmp.o $@
	@rm boot/bootsect.tmp.o

boot/setup.o: boot/setup.s
	@$(AS) -o $@ $<

boot/setup: boot/setup.o
	@$(LD) -Ttext 0x0 -s -oformat=binary -o boot/setup.tmp.o $<
	@objcopy -O binary -j .text boot/setup.tmp.o $@
	@rm boot/setup.tmp.o

# 生成32位ELF
boot/head.o: boot/head.s
	@$(AS) --32 -o $@ $<

# 生成32位ELF
init/main.o: init/main.c
	@$(CC) $(CFLAGS) -m32 -c -o $@ $<

# 链接得到system模块
system: boot/head.o init/main.o
	@$(LD) $(LDFLAGS) -Ttext 0x0 -oformat=binary -o system.tmp $^
	@objcopy -O binary -j .text system.tmp $@
	@rm system.tmp

tools/build: tools/build.c
	@$(CC) $< -o $@

$(HD_IMG):
	@dd if=/dev/zero of=$@ bs=512 count=20480

clean:
	@echo "开始清理..."
	@rm $(OBJECTS)
	@echo "清理完毕!"
