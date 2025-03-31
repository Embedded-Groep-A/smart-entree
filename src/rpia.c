#include <stdio.h>
#include <stdlib.h>
#include <wiringSerial.h>

int main() {
    const char *serial_port = "/dev/ttyS0"; // Replace with your serial port
    int baud_rate = 9600; // Set baud rate

    // Open the serial port
    int fd = serialOpen(serial_port, baud_rate);
    if (fd == -1) {
        perror("Unable to open serial port");
        return 1;
    }

    printf("Serial port opened successfully.\n");

    while (1) {
        // Check if data is available to read
        if (serialDataAvail(fd)) {
            char c = serialGetchar(fd); // Read a character
            printf("Received: %c\n", c);
            fflush(stdout); // Ensure output is printed immediately
        }
    }

    serialClose(fd); // Close the serial port
    return 0;
}
