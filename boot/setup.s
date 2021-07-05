/* setup.s (C) kfggww */

    .equ INITSEG, 0x9000
    .equ SYSSEG, 0x1000
    .equ SETUPSEG, 0x9020

    .code16
    .text

    .global _start, start
_start:
start:
    movw $INITSEG, %ax
    movw %ax, %ds               /*确保ds指向的是INITSEG, 需要在这个段里保存机器参数, bootsect任务完成, 可以废弃*/

    /*使用BIOS中断获取机器参数*/
    /*内存*/
    movb $0x88, %ah             /*TODO: 似乎不能获取全部的内存, 看是否有其他方式*/
    int $0x15
    movw %ax, 2                 /*获取拓展内存大小, 单位是KB, ax中保存了结果*/

    /*TODO: 显示参数*/

    /*TODO: 硬盘参数*/


    /*关中断*/
    cli
    /*TODO: 8259重新编程, 到底是在干什么?*/

    /*移动system到0地址*/
    cld
    movw $0x0000, %ax

do_move:
    movw %ax, %es
    addw $0x1000, %ax
    movw %ax, %ds
    cmpw $0x9000, %ax
    jz end_move
    xor %si, %si
    xor %di, %di
    movw $0x8000, %cx
    rep movsw
    jmp do_move


end_move:
    /*必须要设置ds, 使其指向SETUPSEG段, 否则加载gdt和idt时会出错*/
    movw $SETUPSEG, %ax
    movw %ax, %ds

    /*准备进入保护模式*/

    /*1. 加载gdt, idt*/
    lgdt gdt_48
    lidt idt_48         /*TODO: 填充idt表*/

    /*2. 打开A20地址线.*/
    in $0x92, %al       /*参考osdev, TODO: 确认是否通用*/
    or $2, %al
    out %al, $0x92

    /*3. 使能CR0的PE位*/
    movl $0x00000001, %eax
    movl %eax, %cr0

    /*4. 跳转到system模块的head处开始执行*/
    jmp $0x8, $0

    /*gdt表的内容, 每个表项占8个字节*/
gdt:
    /*空描述符*/
    .word 0, 0, 0, 0    /*第一个描述符为空*/

    /*代码段描述符*/
    .word 0x07ff
    .word 0x0000
    .word 0x9a00
    .word 0x00c0

    /*数据段描述符*/
    .word 0x07ff
    .word 0x0000
    .word 0x9200
    .word 0x00c0

    /*idt表的内容*/
idt:


gdt_48:
    .word 0x800
    /*.word 512 + gdt, 0x9*/        /*这里是linux0.12的写法, 需要注意字节序*/
    .long (SETUPSEG << 4 + gdt)     /*修改后的写法, 不必考虑字节序, 只要计算出地址填上就可以*/
idt_48:
