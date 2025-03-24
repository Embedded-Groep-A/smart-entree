// RPI MET I2C NAAR L432KC {raspberrypi}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include "pisocket.h"

#define PORT 8069
#define HOST "bramsvoorhoofd.local"
#define I2C_ADDR 0x11 // Replace with your I2C device address

uint8_t rgbValues[3] = {255, 255, 255};

int main() {
    int client_fd = connectSocket(HOST, PORT);

    int i2c_fd = wiringPiI2CSetup(I2C_ADDR);


    // Read data from I2C device
    int data = wiringPiI2CRead(i2c_fd);

    printf("Received data: %d\n", data);

    return 0;
}
