INSTALLDIR= modules
ifneq ($(KERNELRELEASE),)
	obj-m := shillu.o
	shillu-objs := init.o clean_up.o close_dev.o open_dev.o write_dev.o read_dev.o lseek_dev.o 

else
	KERNDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules
	@rm -rf $(INSTALLDIR)
	@mkdir $(INSTALLDIR)
	@mv *.ko *.mod.c *.o .*.cmd $(INSTALLDIR)

clean:
	rm -rf $(INSTALLDIR)
	rm -rf modules.order
	rm -rf Module.symvers
endif

