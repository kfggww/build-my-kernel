obj-m += hello_kernel.o

KERNEL_RELEASE ?= $(shell uname -r)
KDIR = /lib/modules/$(KERNEL_RELEASE)/build
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
