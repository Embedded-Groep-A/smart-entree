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
    uint8_t rgbValues[3] = {255, 0, 0};

    while (1) {
        sendToServer(client_fd, RGBLED, rgbValues, 3);
        sleep(1);

    }
    return 0;
}



