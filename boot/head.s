/* head.s (C) kfggww
 * system模块的开始部分, 用于重新设置gdt, idt, 分页, 并检查A20是否真正开启了, 然
 * 后将控制交给main函数.
 */

    .code32
    .text
    .global _start, start

_pg_dir: /*页目录表会写在此处*/

_start:
start:
    /*重置数据段选择子*/
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    /*TODO: 设置ss*/

    /*设置idt和gdt*/
    call set_idt
    call set_gdt    /*之前的gdt在setup所在的段*/

    /*再次设置数据段选择子, 因为gdt刚刚被重置了*/
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    /*TODO: 设置ss*/


    /*检查A20是否开启.*/
    xor %eax, %eax
check_a20:
    movl %eax, 0x00000
    movl 0x100000, %ebx
    inc %eax
    cmpl 0x00000, %ebx
    jz check_a20            /*如果地址0和地址1M处的值始终相等, 证明A20未开启*/

    jmp after_pages_table

    /*TODO: 检查x87协处理器, 似乎没什么用, 暂且忽略*/

    /*以下的4页用来存放页表*/
    .org 0x1000
pg0:
    .org 0x2000
pg1:
    .org 0x3000
pg2:
    .org 0x4000
pg3:
    .org 0x5000


    /*以上代码会被设置成系统的页表*/
after_pages_table:
    pushl $0
    pushl $0
    pushl $0
    pushl $L6
    pushl $main
    /*setup_paging中包含一条ret指令, 执行ret时回从栈中取出main的地址开始执行*/
    jmp setup_paging

    /*一旦main函数返回, 说明系统出错, 则在此处死机*/
L6:
    jmp L6



    /*设置中断描述符表idt*/
    /*用默认中断处理函数给idt表填充表项*/
set_idt:
    leal ignore_int, %edx   /*加载中断处理函数地址*/
    movl $0x00080000, %eax  /*设置段选择子*/
    movw %dx, %ax           /*此时eax包含中断门的低4字节*/
    movw $0x8e00, %dx       /*此时edx包含中断门的高4字节*/
    /*循环设置idt表项*/
    movl $256, %ecx
    leal idt, %edi
set_idt_entry:
    movl %eax, 0(, %edi, 1)
    movl %edx, 4(, %edi, 1)
    addl $8, %esi
    loop set_idt_entry
    ret


    /*设置gdt*/
set_gdt:
    lgdt gdt_48
    ret


    /*默认中断处理函数, 直接返回, 啥也不干.*/
ignore_int:
    iret

    /*设置页表*/
setup_paging:
    /*首先清零0地址开始的5页内容*/
    movl $1024*5, %ecx
    xor %eax, %eax
    xor %edi, %edi
    cld
    rep
    stosl

    /*设置页目录表的表项*/
    movl $pg0+0x7, _pg_dir
    movl $pg1+0x7, _pg_dir+4
    movl $pg2+0x7, _pg_dir+8
    movl $pg3+0x7, _pg_dir+12


    /* 填充页表项.
	 * 从pg3页表的最后一个表项开始填充, 最后一页的线性地址是0xfff000, 属性设置为
	 * 0x7
	 */
    movl $0xfff007, %eax
    movl $4*(1024*5-1), %edi
    /*设置edi递减的*/
    std
setup_page_entry:
    stosl
    subl $4096, %eax
    cmpl $4096, %edi
    jnz setup_page_entry

    /*设置cr3寄存器*/
    xor %eax, %eax
    movl %eax, %cr3

    /*开启分页模式*/
    movl %cr0, %eax
    or $0x80000000, %eax
    movl %eax, %cr0

    /*返回. 从栈中弹出main函数地址并开始执行*/
    ret



    /*Intel手册建议idt表8字节对齐*/
    .align 8
    /*idt表的内容, 先填上0*/
idt:
    .fill 256, 8, 0

    /*gdt表的内容, 0: null, 1: code, 2: data, 3: 保留; 之后的留给系统创建的任务.*/
gdt:
    .quad 0x0000000000000000
    .quad 0x00c09a0000000fff    /*代码段, 16M, 可读可执行*/
    .quad 0x00c0920000000fff    /*数据段, 16M, 可读可写*/
    .quad 0x0000000000000000    /*保留不用*/
    .fill 252, 8, 0             /*这里只设置了256个表项, 实际可以更多*/


idt_48:
    .word 256*8-1
    .long idt

gdt_48:
    .word 256*8-1
    .long gdt
