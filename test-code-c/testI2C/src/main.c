#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>     // For sleep()
#include <fcntl.h>      // Contains file controls for open()
#include <termios.h>    // Contains POSIX terminal control definitions
#include <errno.h>      // Error integer and strerror() function
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int i2c_fd;
// The I2C device.
const char * device = "/dev/i2c-0";
// Set the I2C address (the device number).
const int dev_address = 0x68; //address was read via i2cdetect command

extern int errno;

int main()
{
    printf("+----------------------------------------+\n");
    printf("|  Test Embedded Linux I2C               |\n");
    printf("|   - Read time from RTC DS2331          |\n");
    printf("+----------------------------------------+\n");

    //Open device
    int i2c_fd = open(device, O_RDWR | O_NONBLOCK);
    if (i2c_fd < 0){
        printf("Device port openning error: [%i] %s\n", errno, strerror(errno));
        return errno;
    }else{
        printf("%s opened successfully\n", device);
    }

    // set the I2C slave address for all subsequent I2C device transfers
    if (ioctl(i2c_fd, I2C_SLAVE, dev_address) < 0){
        printf("Slave address set error: [%i] %s\n", errno, strerror(errno));
        return errno;
    }else{
        printf("0x%x slave address is set\n", dev_address);
    }

    char data_buffer[10];

    for(int i = 0; i < 100; i++){
        memset(&data_buffer, '\0', sizeof(data_buffer));
        write(i2c_fd, data_buffer, 1);
        int read_bytes = read(i2c_fd, &data_buffer, 6); //Read 6 bytes: yy-mm-dd hh:mm:ss

        printf("Time from rtc board: %X:%X:%X %X/%X/20%02X\n", data_buffer[2], data_buffer[1], data_buffer[0],
                                                data_buffer[3], data_buffer[4], data_buffer[5]);
        sleep(1);
    }

    close(i2c_fd);

    printf("Exit Test I2C\n");
    return 0;
}
