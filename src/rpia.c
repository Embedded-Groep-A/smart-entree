// RPI MET I2C NAAR L432KC {raspberrypi}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../lib/pisocket/include/pisocket.h"

#define PORT 8080
#define HOST "raspberrypi.local"


int main() {
    int client_fd = connectSocket(HOST, PORT);

    sendData(client_fd, BUTTON, 1);

    disconnectSocket(client_fd);
    return 0;
}



