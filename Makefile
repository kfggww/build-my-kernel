# TINIX

AS = as
CC = gcc
LD = ld

HD_IMG := hd10m.img
OBJECTS := boot/*.o $(HD_IMG) boot/bootsect


.PHONY: all clean

all: Image

Image: tools/build $(HD_IMG) boot/bootsect
	@tools/build boot/bootsect boot/setup system $(HD_IMG)


#boot/bootsect: boot/bootsect.o
#	@objcopy -O binary -j .text $< $@

# 参考linux 2.4.1的Makefile
boot/bootsect.o: boot/bootsect.s
	@$(AS) -o $@ $<

boot/bootsect: boot/bootsect.o
	@$(LD) -Ttext 0x0 -s -oformat=binary -o boot/bootsect.tmp.o $<
	@objcopy -O binary -j .text boot/bootsect.tmp.o $@
	@rm boot/bootsect.tmp.o

tools/build: tools/build.c
	@$(CC) $< -o $@

$(HD_IMG):
	@dd if=/dev/zero of=$@ bs=512 count=20480

clean:
	@echo "开始清理..."
	@rm $(OBJECTS)
	@echo "清理完毕!"
