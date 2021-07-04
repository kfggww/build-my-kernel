# TINIX

## 目标

模仿linux-0.12实现一个简单的内核.


## 参考
1. [linux 0.12](http://www.oldlinux.org/Linux.old/kernel/0.1x/linux-0.12/)
2. [linux 2.4](https://mirrors.edge.kernel.org/pub/linux/kernel/v2.4/)
3. [ELF file format](https://refspecs.linuxfoundation.org/elf/elf.pdf)
4. [AT&T assembly](https://gist.github.com/mishurov/6bcf04df329973c15044)
5. [BIOS Interrupt in detail](https://stanislavs.org/helppc/int_table.html)

## 进展
- 实现了bootsect读取后续模块




## TODOs
- 实现setup模块
- 实现head模块




## 汇编语言坑点
- 乘法指令. mul, 无符号乘法, 且有隐含操作数
- 汇编和链接的过程. 对于一个汇编文件, 单纯使用as汇编得到的目标文件是可重定位文件, 其中一些引用的符号还没有值, 或者说只有默认的0值, 需要经过链接之后, 才可以得到这些符号的值. 具体在使用as和ld时需要注意的地方可以参考linux-2.4.1.
