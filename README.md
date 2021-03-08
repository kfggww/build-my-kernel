# build-my-kernel
Learn how does the linux kernel work

## Day 0: 如何编写内核模块

1. 安装必要的包, 在Arch下, 需要安装"linux-headers"

2. 写模块文件
   - 模块加载函数
   - 模块卸载函数
   - MODULE_LICENSE
   - Makefile
     这里的Makefile实际上会使用/lib/modules/$(KERNEL_RELEASE)/build下的Makefile