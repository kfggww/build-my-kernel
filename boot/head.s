/*
* 使用Multiboot2标准引导内核.
*
*/

    .align 8
    .text
    .global start, _start
start:
_start:
    jmp multiboot2_entry

/*MULTIBOOT2 HEADER BEGIN*/
    .align 8
multiboot2_header:
    .long 0xE85250D6
    .long 0
    .long multiboot2_header_end - multiboot2_header
    .long -(0xE85250D6 + 0 + multiboot2_header_end - multiboot2_header)

    .align 8
addr_tag:
    .short 2
    .short 0
    .long addr_tag_end - addr_tag
    .long multiboot2_header
    .long start
    .long _edata
    .long _end
addr_tag_end:

    .align 8
entry_addr_tag:
    .short 3
    .short 0
    .long entry_addr_tag_end - entry_addr_tag
    .long multiboot2_entry
entry_addr_tag_end:

    .align 8
    .short 0
    .short 0
    .long 8

multiboot2_header_end:
/*MULTIBOOT2 HEADER END*/

/*入口点*/
multiboot2_entry:
loop:
    hlt
    jmp loop
