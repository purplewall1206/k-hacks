obj-m := proclist.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -f *.mod.c *.ko *.o

run:
	sudo insmod proclist.ko
	sudo dmesg
	sudo rmmod proclist