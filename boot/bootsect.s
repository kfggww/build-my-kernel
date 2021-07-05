/*
 * bootsect.s (C) kfggww
 * 由BIOS加载的第一个扇区, 用于加载内核的其他部分.
 */


	/*内核各部分的段地址.*/
	.equ BOOTSEG, 0x7c0
	.equ INITSEG, 0x9000
	.equ SETUPSEG, 0x9020
	.equ SYSSEG, 0x1000
	.equ SETUPLEN, 4	/*setup模块长度, 暂时设置成和linux 0.12相同*/

	/*最初机器处于real mode.*/
	.code16
	.text
	
	.global _start, start
	/*将启动扇区移动到INITSEG位置.*/
_start:
start:
	movw $BOOTSEG, %ax
	movw %ax, %ds
	movw $INITSEG, %ax
	movw %ax, %es
	movw $256, %cx
	xor %si, %si
	xor %di, %di
	cld		/*确保si和di的方向是递增的*/
	rep movsw
	ljmp $INITSEG, $go	/*跳转到新的位置继续执行*/

	/*设置各个段寄存器的值*/
go:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw $512, %bx
	movw %ax, %ss
	movw $0xfef4, %sp	/*这里是否也可以设置成其他的值?*/

	/* 使用BIOS 0x13中断, 从硬盘上读取setup.
	 * ah = 0x02, 表示读扇区, al: 要读的扇区数
	 * ch: 磁道号, cl: 扇区号
	 * dh: 磁头号, dl: 驱动器号软盘从0x0开始, 硬盘从0x80开始
	 * 接收数据的内存地址为: es:bx
	*/
	movw $0x0200 + SETUPLEN, %ax
	movw $0x0002, %cx
	movw $0x0080, %dx
	int $0x13
	jnc ok_load_setup
	/*TODO: 加载setup的出错处理.*/


	/*加载system.*/
ok_load_setup:
	/*获取光标位置*/
	movb $0x03, %ah	/*功能号*/
	xor %bh, %bh	/*页号*/
	int $0x10		/*获取的位置存放在dx中, dh行, dl列*/

	/*写提示信息, 因为system模块相对较大, 读取的时间会长一点*/
	movw $INITSEG, %ax
	movw %ax, %es		/*恢复es的值*/
	movw $msg1, %bp		/*es:bp确定了要显示字符串的位置*/
	movw $0x1301, %ax	/*ah = 0x13, 功能号; al=0x01, 使用bl规定的显示属性*/
	movw $0x0007, %bx	/*设置显示属性. bh: 页号, bl: 显示属性*/
	movw $18, %cx		/*设置字符串个数*/
	int $0x10

	/*读取system模块*/
	call read_it

	/*转到setup执行.*/
	jmp $SETUPSEG, $0

	/*功能函数: 从硬盘上读取system模块, 使用LBA的方式读取
	* system模块最大512KB, 共1024个扇区, 每次读取128个扇区.
	*/
read_it:
	movw $0, %si
begin_read_it:
	cmpw $8, %si
	jz ok_read_it
	/*读取扇区个数*/
	movw $0x1f2, %dx
	movb $0x80, %al	/*每次读取64KB, 128个扇区*/
	out %al, %dx	/*写数据到端口*/

	/*设置开始扇区编号, LBA28*/
	movw %si, %ax
	movw $128, %dx
	mulw %dx
	addw $1 + SETUPLEN, %ax
	movw $0x1f3, %dx
	out %al, %dx

	inc %dx
	movb %ah, %al
	out %al, %dx	/*写端口0x1f4*/

	xor %ax, %ax
	inc %dx
	out %al, %dx	/*写端口0x1f5*/

	inc %dx
	out %al, %dx	/*写端口0x1f6*/

	/*请求读*/
	inc %dx
	movb $0x20, %al
	out %al, %dx	/*写端口0x1f7*/

	/*等待读完成, 通过查询0x1f7端口状态确定*/
wait_lba_read:
	in %dx, %al
	andb $0x88, %al
	cmpb $0x08, %al
	jnz wait_lba_read

	/*取出数据*/
	movw %si, %ax
	movw $0x1000, %bx
	mulw %bx
	addw $SYSSEG, %ax
	movw %ax, %es						/*设置es*/
	movw $0, %bp						/*设置bx, es:bp确定数据要写入的内存位置*/

	movw $0x8000, %cx
	movw $0x1f0, %dx					/*读数据端口, 乘法指令影响dx*/
copy_data:
	in %dx, %ax
	movw %ax, %es:(%bp)
	addw $2, %bp
	loop copy_data

	inc %si
	jmp begin_read_it
ok_read_it:
	ret

msg1:
	.asciz "\r\nLoading TINIX..."
boot_tag:
	.org 510
	.byte 0x55, 0xaa
