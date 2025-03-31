// Raspberry Pi C Code (RPi_STM32_USART_Communication.c)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define UART_PATH "/dev/ttyS0"

int main() {
    int uart_fd = open(UART_PATH, O_RDWR | O_NOCTTY);
    if (uart_fd < 0) {
        perror("Error opening UART");
        return -1;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);

    const char *message = "Hello from Raspberry Pi!";
    write(uart_fd, message, strlen(message));

    char buffer[100];
    int n = read(uart_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(uart_fd);
    return 0;
}

// Compile with: gcc -o rpi_uart RPi_STM32_USART_Communication.c
// Run with: sudo ./rpi_uart
