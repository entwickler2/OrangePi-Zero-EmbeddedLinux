cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc)

project(embLinux-testUsart LANGUAGES C)

add_executable(embLinux-testUsart src/main.c)
#add_library(stdio 
#	/home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include/stdio.h
#)


