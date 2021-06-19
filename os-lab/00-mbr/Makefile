.PHONY: all clean

OBJECTS := hd.img disk_writer mbr.bin
MBR_BIN_FILE := mbr.bin
HD_IMG_FILE := hd.img

all: write_mbr

clean:
	rm $(OBJECTS)

write_mbr: disk_writer mbr.bin hd.img
	./disk_writer $(MBR_BIN_FILE) $(HD_IMG_FILE) 0

disk_writer: disk_writer.c
	gcc disk_writer.c -o disk_writer

mbr.bin: mbr.asm
	nasm -f bin mbr.asm -o $(MBR_BIN_FILE)

hd.img:
	dd if=/dev/zero of=$(HD_IMG_FILE) bs=512 count=20480
