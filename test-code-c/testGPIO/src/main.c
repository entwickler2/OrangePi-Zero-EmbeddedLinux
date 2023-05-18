#include <stdio.h>
#include <string.h>
#include <unistd.h>     // For sleep()
#include <termios.h>    // Contains POSIX terminal control definitions
#include <errno.h>      // Error integer and strerror() function
#include "gpiod.h"

extern int errno;

int main()
{
    printf("+---------------------------------+\n");
    printf("| Test Embedded Linux GPIO (LEDs) |\n");
    printf("|   - blink GPIO10 (PA10)         |\n");
    printf("+---------------------------------+\n");

    const char *chipname = "/dev/gpiochip0";     //device address
    const unsigned int gpio_num = 10;
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int req;

    chip = gpiod_chip_open(chipname);
    if (!chip){
        printf("Getting chip error: [%i] %s\n", errno, strerror(errno));
        return -1;
    }
    printf("%s Opened Successfully\n", chipname);

    line = gpiod_chip_get_line(chip, gpio_num);   //GPIO10(PA10)
    if (!line) {
        printf("Getting line error: [%i] %s\n", errno, strerror(errno));
        gpiod_chip_close(chip);
        return -1;
    }

    req = gpiod_line_request_output(line, "gpio_test", 0);
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
