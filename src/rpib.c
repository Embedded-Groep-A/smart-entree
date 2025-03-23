#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "pisocket.h"
#include <sys/select.h>

#define PORT 8069
#define MAX_CLIENTS 4

int main() {
    int server_fd = hostSocket(PORT);
    initClientArray();

    fd_set read_fds;
    struct timeval timeout;

    while (1) {
        printf("1");
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        int max_fd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket > 0) {
                FD_SET(clients[i].socket, &read_fds);
                if (clients[i].socket > max_fd) {
                    max_fd = clients[i].socket;
                }
            }
        }

        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (FD_ISSET(server_fd, &read_fds)) {
            int client_fd = acceptClient(server_fd);
        }

        // for (int i = 0; i < MAX_CLIENTS; i++) {
        //     if (clients[i].socket > 0 && FD_ISSET(clients[i].socket, &read_fds)) {
        //         enum DataType type;
        //         uint8_t receivedData[4];
        //         size_t receivedSize;

        //         if (listenForData(clients[i].socket, &type, receivedData, &receivedSize) > 0) {
        //             switch (type) {
        //                 case BUTTON:
        //                 case SENSOR: {
        //                     int32_t value;
        //                     memcpy(&value, receivedData, sizeof(value));
        //                     printf("Received %s: %d\n", (type == BUTTON) ? "BUTTON" : "SENSOR", value);
        //                     break;
        //                 }
        //                 case RGBLED:
        //                     printf("Received RGB: %u %u %u\n", receivedData[0], receivedData[1], receivedData[2]);
        //                     break;
        //             }
        //         } else {
        //             printf("Client %d disconnected\n", clients[i].id);
        //             closeClient(clients[i].socket);
        //             clients[i].socket = 0;
        //             clients[i].id = -1;
        //         }
        //     }
        // }

        printf("6");
        fd_set input_fds;
        FD_ZERO(&input_fds);
        FD_SET(STDIN_FILENO, &input_fds);

        if (select(STDIN_FILENO + 1, &input_fds, NULL, NULL, &timeout) > 0 && FD_ISSET(STDIN_FILENO, &input_fds)) {
            char input[20];
            if (fgets(input, sizeof(input), stdin) != NULL) {
                uint8_t rgbValues[3];
                sscanf(input, "%hhu %hhu %hhu", &rgbValues[0], &rgbValues[1], &rgbValues[2]);
                sendToClient(0, RGBLED, (void *)rgbValues, sizeof(rgbValues));
                printf("send func exited\n");
            }
        }

        printf("7");
    }
}
```