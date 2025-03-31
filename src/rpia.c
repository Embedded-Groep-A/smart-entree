#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
    const char *serial_port = "/dev/ttyS0"; // Replace with your serial port
    int fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror("Unable to open serial port");
        return 1;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Configure serial port
    cfsetispeed(&options, B9600); // Set input baud rate
    cfsetospeed(&options, B9600); // Set output baud rate
    options.c_cflag |= (CLOCAL | CREAD); // Enable receiver and set local mode
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE; // Clear current data size setting
    options.c_cflag |= CS8; // 8 data bits
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    options.c_oflag &= ~OPOST; // Raw output

    tcsetattr(fd, TCSANOW, &options);

    char buffer[256];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            printf("Received: %s\n", buffer);
        } else if (bytes_read < 0) {
            perror("Error reading from serial port");
            break;
        }
    }

    close(fd);
    return 0;
}