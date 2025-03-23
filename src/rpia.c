// RPI MET I2C NAAR L432KC {raspberrypi}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "pisocket.h"

#define PORT 8069
#define HOST "bramsvoorhoofd.local"


int main() {
    int client_fd = connectSocket(HOST, PORT);
    uint8_t rgbValues[3] = {0, 85, 170};

    while (1) {
        sendToServer(client_fd, RGBLED, rgbValues, 3);
        rgbValues[0] = (rgbValues[0] + 1) % 256;
        rgbValues[1] = (rgbValues[1] + 1) % 256;
        rgbValues[2] = (rgbValues[2] + 1) % 256;
        usleep(100000);

    }
    return 0;
}



