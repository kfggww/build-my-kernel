/*
* 从实模式进入保护模式的流程. 这些过程不可能在512B大小的代码内完成, 因此需要用
* MBR当跳板, 加载实际的内核代码, 这种情况下, MBR上的代码就相当于是bootloader了
*/


    # 准备GDT

    # 关闭中断

    # 打开A20地址线

    # 启动保护模式

    # 准备页目录表和页表

    # 启动分页

/*
* 问题:
* 1. 如果内核的尺寸大于1M怎么办? 实模式下寻址范围有限, 此时是不是就需要把设置保护模
* 式的代码单独制作成一个模块来加载, 模式转换完毕之后在去加载实际的内核.
*
* 2. 如果使用GRUB2这个bootloader, 需要做些什么? 即GRUB2和内核之间需要遵守什么样的协
* 议才能协调工作, 如何识别内核? 内核加载到哪里? 保护模式的切换谁来做? 如何向内核
* 传递一些启动参数?
*/
