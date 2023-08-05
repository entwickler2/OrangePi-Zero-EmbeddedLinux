- Check that code conforms to Linux Kernel Coding Style Rules:
	Run command:
		<path to kernel source directory>/scripts/checkpatch.pl -f --no-tree <momdule-file-name.c>

- Compile kernel module:
  1.	Make sure that KDIR variable in the MakeFile points to correct kernel source directory
	Note: KDIR defined in the MakeFile
  2.	Make sure that CROSS_COMPILE variable points to path with cross compiler preffix name, f.e. <path to gcc cross compiler>/arm-buildroot-linux-gnueabihf-
	Note: CROSS_COMPILE variable is external
  3.	Examle of command:
	make CROSS_COMPILE=~/Documents/Projects/embeddedLinux/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf- ARCH=arm
