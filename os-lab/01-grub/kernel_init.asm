;;; 内核初始化代码
;;; 编译之后的bin文件可以被GRUB2加载, 注意GRUB2要求各个Tag之间8字节对齐, 这在文
;;; 档中是没有说明的. 启动bochs之后, 会发现eax和ebx的值按照代码中的指令被设置了.
MBT_HDR_FLAGS	EQU 0x00010003
MBT_HDR_MAGIC	EQU 0x1BADB002 ;多引导协议头魔数
MBT_HDR2_MAGIC	EQU 0xe85250d6 ;第二版多引导协议头魔数

[bits 32]
_start:
    jmp _entry

ALIGN 8
mbt2_hdr:
	DD	MBT_HDR2_MAGIC
	DD	0
	DD	mbt2_hdr_end - mbt2_hdr
	DD	-(MBT_HDR2_MAGIC + 0 + (mbt2_hdr_end - mbt2_hdr))
ALIGN 8
	DW	2, 0
	DD	24
	DD	mbt2_hdr
	DD	_start
	DD	0
	DD	0
ALIGN 8
	DW	3, 0
	DD	12
	DD	_entry
ALIGN 8
	DW	0, 0
	DD	8
mbt2_hdr_end:

_entry:
	mov eax, 0x12345678
	mov ebx, 0x12345678
	hlt
