#include <stdio.h>
#include <string.h>
#include <fcntl.h>  // Contains file controls for open()
#include <unistd.h> // For close()
#include <termios.h> // Contains POSIX terminal control definitions
#include <errno.h>  // Error integer and strerror() function
#include <signal.h>

static volatile int keepRunning = 1;
void intHandler(int) {
    keepRunning = 0;
}

int main()
{
    printf("+--------------------------------+\n");
    printf("|    Test Embedded Linux USART   |\n");
    printf("|     - UART1 send/receive       |\n");
    printf("+--------------------------------+\n");

   signal(SIGINT, intHandler);

    struct termios ttySettings; // Create new termios struct to get/set device settings
    char read_buf [256];    // Memory for read buffer, set size according to your needs
    const char *device_id = "/dev/ttyS1";   //Name of linux device

    extern int errno;
    int tty_fd;

    // Open the serial port.
    tty_fd = open(device_id, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (tty_fd < 0){
        printf("Serial port openning error: [%i] %s\n", errno, strerror(errno));
    }else{
        printf("%s Opened Successfully\n", device_id);
    }

    // Read in existing settings, and handle any error
    if (tcgetattr(tty_fd, &ttySettings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    //Set needed configuration to structure
    printf("Stop bits:  1\n");
    printf("Parity bit:  none\n");
    ttySettings.c_cflag &= ~PARENB;     // Clear parity bit, disabling parity (most common)
    ttySettings.c_cflag &= ~CSTOPB;     // Clear stop field, only one stop bit used in communication (most common)
    ttySettings.c_cflag &= ~CSIZE;      // Clear all bits that set the data size
    ttySettings.c_cflag |= CS8;         // 8 bits per byte (most common)
    ttySettings.c_cflag &= ~CRTSCTS;    // Disable RTS/CTS hardware flow control (most common)
    ttySettings.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    ttySettings.c_lflag &= ~ICANON;
    ttySettings.c_lflag &= ~ECHO;       // Disable echo
    ttySettings.c_lflag &= ~ECHOE;      // Disable erasure
    ttySettings.c_lflag &= ~ECHONL;     // Disable new-line echo
    ttySettings.c_lflag &= ~ISIG;       // Disable interpretation of INTR, QUIT and SUSP
    ttySettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    ttySettings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    ttySettings.c_oflag &= ~OPOST;      // Prevent special interpretation of output bytes (e.g. newline chars)
    ttySettings.c_oflag &= ~ONLCR;      // Prevent conversion of newline to carriage return/line feed
    // ttySettings.c_oflag &= ~OXTABS;  // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // ttySettings.c_oflag &= ~ONOEOT;  // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
    ttySettings.c_cc[VTIME] = 2;       // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    ttySettings.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 115200
    printf("Baud rate:  115200\n");
    cfsetispeed(&ttySettings, B115200);
    cfsetospeed(&ttySettings, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(tty_fd, TCSANOW, &ttySettings) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }
    printf("+--------------------------------+\n");


    // Set read buffer to 0 to allow calling printf() without issues.
    memset(&read_buf, '\0', sizeof(read_buf));

    int i = 0;
    while(keepRunning){
	i++;
        // Write data to serial port
        char msg[20] = { '\0' };
        sprintf(msg, "Test message: %d\n\r", i);

        printf("SEND: %s", msg);

        write(tty_fd, msg, strlen(msg));
        sleep(3);   //Delay between sending messages

        // Read bytes. The behaviour of read() depends on the configuration
        // settings above, specifically VMIN and VTIME
        int num_bytes = read(tty_fd, &read_buf, sizeof(read_buf));

        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
            //printf("Error reading: %s", strerror(errno));
            //return 1;
        }else{
            // Print received ASCII data
            printf("RECEIVE: [%i] bytes. Received message: %s\n", num_bytes, read_buf);
        }
    }

    close(tty_fd);

    return 0;
}
