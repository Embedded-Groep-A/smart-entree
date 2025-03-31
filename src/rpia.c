#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>

int main() {
    const char *serial_port = "/dev/ttyS0"; // Replace with your serial port
    int baud_rate = 115200; // Set baud rate

    // Open the serial port
    int fd = serialOpen(serial_port, baud_rate);
    if (fd == -1) {
        perror("Unable to open serial port");
        return 1;
    }

    printf("Serial port opened successfully.\n");

    char buffer[256]; // Buffer to store a line
    int index = 0;

    while (1) {
        // Check if data is available to read
        if (serialDataAvail(fd)) {
            char c = serialGetchar(fd); // Read a character
            if (c == '\n') { // End of line
                buffer[index] = '\0'; // Null-terminate the string
                printf("Received line: %s\n", buffer);
                fflush(stdout); // Ensure output is printed immediately
                index = 0; // Reset buffer index for the next line
            } else if (index < sizeof(buffer) - 1) {
                buffer[index++] = c; // Add character to buffer
            }
        }
    }

    serialClose(fd); // Close the serial port
    return 0;
}
