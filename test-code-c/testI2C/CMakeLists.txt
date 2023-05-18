cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc)
SET(DIR_TO_LIB_HEADERS /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/)

include_directories(${DIR_TO_LIB_HEADERS})

SET(PROJECT_NAME embLinux-testI2C)

project(${PROJECT_NAME} LANGUAGES C)

add_executable(${PROJECT_NAME} src/main.c)



