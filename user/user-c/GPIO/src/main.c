#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>    // Contains POSIX terminal control definitions
#include <errno.h>
#include "gpiod.h"

char *used_chipname = "/dev/gpiochip0";     //default device address
unsigned int gpio_num = 10;                 //default gpio line
struct gpiod_chip *chip;
struct gpiod_line *line;
extern int errno;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "d:l:")) != EOF){
        switch(opt){
            case 'd':
                printf("Selected device address: \"%s\"\n", optarg);
                used_chipname = optarg;
                break;
            case 'l':
                printf("Selected gpio line: \"%s\"\n", optarg);
                gpio_num = atoi(optarg);
                break;
            default:
                printf("Usage:\n "
                       "1: no arguments, will be used default '/dev/gpiochip0' device address and default gpio line 10 (PA10)\n"
                       "2: 'programm_name -d /dev/gpiochipN -l L', where N - gpio chip number, L - gpio line number\n"
                       "3: specified only -d option with argument\n"
                       "4: specified only -l option with argument\n");
                break;
        }
    }

    //TODO: Add checking of input parameters, device contains correct path and name, gpio line is a number
    //TODO: Add Cntl+C signal hanler 


    printf("+---------------------------------+\n");
    printf("| Test Embedded Linux GPIO (LEDs) |\n");
    printf("|   - output mode                 |\n");
    printf("+---------------------------------+\n");

    chip = gpiod_chip_open(used_chipname);
    if (!chip){
        printf("Getting chip error: [%i] %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }
    printf("%s Opened Successfully\n", used_chipname);

    line = gpiod_chip_get_line(chip, gpio_num);
    if (!line) {
        printf("Getting line error: [%i] %s\n", errno, strerror(errno));
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    int req = gpiod_line_request_output(line, "gpio_test", 0);
    if(req < 0){
        printf("Requesting gpio line to output error: [%i] %s\n", errno, strerror(errno));
        gpiod_line_release(line);
        gpiod_chip_close(chip);
    }

    printf("GPIO%d Configured to ouptut\n", gpio_num);

    for(;;)
    {
        gpiod_line_set_value(line, 1);
        printf("GPIO%d On\n", gpio_num);
        sleep(1);
        gpiod_line_set_value(line, 0);
        printf("GPIO%d Off\n", gpio_num);
        sleep(1);
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);

    printf("Exit Test GPIO\n");
    return EXIT_SUCCESS;
}
