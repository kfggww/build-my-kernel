ifneq ($(KERNELRELEASE),)
obj-m += hello.o
else
KERNELRELEASE ?= $(shell uname -r)
KERN_DIR ?= /lib/modules/$(KERNELRELEASE)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) clean
endif

.PHONY: all clean
