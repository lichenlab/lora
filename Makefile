ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= lora.o
lora-objs := sx1276.o sx1276-LoRa.o sx1276-LoRaMisc.o sx1276-Hal.o rf.o

else

PWD       := $(shell pwd)
KERNELDIR ?= $(PWD)/..

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers


depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
