CC = $(CROSS_COMPILE)gcc
KDIR := ~/Documents/Projects/embeddedLinux/buildroot/output/build/linux-6.1.14

obj-m+=module-1.o

all:
	@echo "INFO: required env variables:"
	@echo "CC: "$(CC)
	@echo "CROSS_COMPILE: "$(CROSS_COMPILE)
	@echo "ARCH: "$(ARCH)
	make -C ${KDIR} M=$(PWD) modules
clean:
	make -C ${KDIR} M=$(PWD) clean
