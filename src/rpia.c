#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>
#include <stdint.h>
#include <unistd.h>
#include "pisocket.h"

#define PORT 8069
#define HOST "rpibentree.local"

uint8_t rgbValues[3] = {0, 85, 170};


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
    int client_fd = connectSocket(HOST, PORT);

    const char *serial_port = "/dev/ttyS0";
    int baud_rate = 115200;

    int fd = serialOpen(serial_port, baud_rate);
    if (fd == -1) {
        perror("Unable to open serial port");
        return 1;
    }

    printf("Serial port opened successfully.\n");

    char buffer[256];
    int index = 0;

    while (1) {
        if (serialDataAvail(fd)) {
            index = readLine(fd, buffer, sizeof(buffer));
            if (index > 0) {
                printf("Received: %s\n", buffer);
                if (strncmp(buffer, "BTN", 3) == 0) {
                    sendToServer(client_fd, RGBLED, rgbValues, 3);
                    rgbValues[0] = (rgbValues[0] + 10) % 256;
                    rgbValues[1] = (rgbValues[1] + 10) % 256;
                    rgbValues[2] = (rgbValues[2] + 10) % 256;
                }
            }

        }

    }

    serialClose(fd);
    return 0;
}
