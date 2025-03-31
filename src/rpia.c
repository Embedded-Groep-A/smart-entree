#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>


char readLine(int fd, char *buffer, int size) {
    int index = 0;
    while (index < size - 1) {
        if (serialDataAvail(fd)) {
            char c = serialGetchar(fd);
            if (c == '\n') {
                break; // End of line
            } else {
                buffer[index++] = c; // Add character to buffer
            }
        }
    }
    buffer[index] = '\0'; // Null-terminate the string
    return index; // Return the number of characters read
}

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
            // Read a line from the serial port
            index = readLine(fd, buffer, sizeof(buffer));
            if (index > 0) {
                printf("Received: %s\n", buffer); // Print the received line
            }
        }
    }

    serialClose(fd); // Close the serial port
    return 0;
}
