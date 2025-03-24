// RPI MET I2C NAAR L432KC {raspberrypi}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "pisocket.h"

#define PORT 8069
#define HOST "bramsvoorhoofd.local"

#define I2C_DEVICE "/dev/i2c-1"
#define I2C_ADDR   0x20  

uint8_t rgbValues[3] = {255, 255, 255};

int main() {
    int file = open(I2C_DEVICE, O_RDWR);
    ioctl(file, I2C_SLAVE, I2C_ADDR);

    while (1) {
        char buffer[6];
        int bytesRead = read(file, buffer, 6);
        if (bytesRead < 0) {
            perror("Failed to read from the i2c bus");
        } else {
            printf("Data read: ");
            for (int i = 0; i < bytesRead; i++) {
            printf("%02x ", buffer[i]);
            }
            printf("\n");
        }

    }

    return 0;
}
