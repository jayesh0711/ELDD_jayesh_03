obj-m += q1.o 

ifdef ARCH
KERN_DIR = /lib/modules/5.15.0-15-generic/build/
else
KERN_DIR = /lib/modules/$(shell uname -r)/build/
PWD = $(shell pwd)
endif

all:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules
	gcc -o read read.c 
	gcc -o write write.c

clean:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) clean
	rm -rf read write