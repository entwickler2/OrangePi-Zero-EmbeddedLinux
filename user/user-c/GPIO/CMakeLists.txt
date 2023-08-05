cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-gcc)
SET(DIR_TO_LIB_HEADERS /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/)
SET(LIBGPIOD_PATH /home/andrii/Documents/Projects/embeddedLinux/buildroot/output/target/usr/lib/libgpiod.so)

include_directories(${DIR_TO_LIB_HEADERS})

SET(PROJECT_NAME embLinux-testGPIO)

project(${PROJECT_NAME} LANGUAGES C)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} ${LIBGPIOD_PATH})



