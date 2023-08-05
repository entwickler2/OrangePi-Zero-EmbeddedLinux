#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>     // Get sleep() function
#include <fcntl.h>      // Contains file controls for open()
#include <termios.h>    // Contains POSIX terminal control definitions
#include <errno.h>      // Error integer and strerror() function
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int i2c_fd;
// The I2C device.
char *device = "/dev/i2c-0";
// Set the I2C address (the device number).
int dev_address = 0x68; //address was read via i2cdetect command
//Set time buffer and vars for parsing argument parameters
int year, month, day, week, hours, minutes, seconds;
char time_buffer[8];
//Set time option
bool set_time_selected = false;

extern int errno;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "a:d:s:")) != EOF){
        switch(opt){
            case 'd':
                device = optarg;
                printf("Selected device name: %s\n", device);
                break;
            case 'a':
                dev_address = atoi(optarg);
                printf("Selected slave address: 0x%02X\n", dev_address);
                break;
            case 's':
                sscanf(optarg, "%x/%x/%x-%x-%x:%x:%x", &month, &day, &year, &week, &hours, &minutes, &seconds);
                printf("Specified time:\n  "
                       "Year: 20%02x\n  "
                       "Month: %02x\n  "
                       "Day: %02x\n  "
                       "Week: %02x\n  "
                       "Hours: %02x\n  "
                       "Minutes: %02x\n  "
                       "Seconds: %02x\n",
                        year, month, day, week, hours, minutes, seconds);
                set_time_selected = true;
                break;
            default:
                printf("Usage:\n"
                       "1: no arguments, will be used default '/dev/i2c-0' device address and default slave address 0x68\n"
                       "2: read current time and set device, slave address: 'programm_name -d /dev/i2c-N -a 0xAA', where N - I2C dev number, AA - i2c slave address in hex\n"
                       "3: read current time and set initial time, set device and slave address: 'programm_name -d /dev/i2c-N -a 0xAA' -s yy:mn:dd-ww:hh:mm:ss,\n"
                       "   where yy - year(0-99), mn - month(1-12), dd - day(1-31), ww - week day number(1-7), hh - hour(0-23), mm - minutes(0-59), ss - seconds(0-59) \n"
                       "4: read current time: specified only -d option with argument\n"
                       "5: read current time: specified only -s option with argument\n");
                break;
        }
    }


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
    memset(&data_buffer, '\0', sizeof(data_buffer));

    //Check set time option
    if (set_time_selected){
        time_buffer[0] = 0x00; //Initial address for writing
        time_buffer[1] = seconds;
        time_buffer[2] = minutes;
        time_buffer[3] = hours;
        time_buffer[4] = week;
        time_buffer[5] = day;
        time_buffer[6] = month;
        time_buffer[7] = year;
        write(i2c_fd, time_buffer, 8); //Set time 
    }

    for(int i = 0; i < 100; i++){
        memset(&data_buffer, '\0', sizeof(data_buffer));
        write(i2c_fd, data_buffer, 1);
        int read_bytes = read(i2c_fd, &data_buffer, 7); //Read time: yy-mm-dd hh:mm:ss

        printf("Time from rtc board: %02X:%02X:%02X %02X/%02X/20%02X\n", data_buffer[2], data_buffer[1], data_buffer[0],
                                                                         data_buffer[4], data_buffer[5], data_buffer[6]);
        sleep(1);
    }

    close(i2c_fd);

    printf("Exit Test I2C\n");
    return 0;
}
