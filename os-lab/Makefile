.PHONY: all clean

all: a.bin hd.img

clean:
	rm a.bin hd.img mbr_writer

a.bin: a.asm
	nasm -f bin a.asm -o a.bin

hd.img: a.bin mbr_writer
	dd if=/dev/zero of=hd.img bs=512 count=20480 && ./mbr_writer

mbr_writer: mbr_writer.c
	gcc mbr_writer.c -o mbr_writer
