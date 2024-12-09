- Check that code conforms to Linux Kernel Coding Style Rules:
	Run command:
		<path to kernel source directory>/scripts/checkpatch.pl -f --no-tree <momdule-file-name.c>

- Compile kernel module:
  1.	Make sure that KDIR variable in the MakeFile points to correct kernel source directory
	Note: KDIR defined in the MakeFile
  2.	Make sure that CROSS_COMPILE variable points to path with cross compiler preffix name, f.e. <path to gcc cross compiler>/arm-buildroot-linux-gnueabihf-
	Note: CROSS_COMPILE variable is external or internal and shall not have 'gcc' ending as CC variable
  3.	Make sure CC veriable is created and points to cross compiller, f.e. <full path to gcc cross compiler name>/arm-buildroot-linux-gnueabihf-gcc
	Note: The CC variable is internal or external, f.e. CC=$(CROSS_COMPILE)gcc
  4.	Make sure ARCH variable is created and external, the variable shall define architecture name, f.e. arm
  5.	Object *.o name need to be the same as module file, *.c.

Examle of command:
	make CROSS_COMPILE=~/Documents/Projects/embeddedLinux/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf- ARCH=arm
	make ARCH=arm  # if CROSS_COMPILE is created in Makefile