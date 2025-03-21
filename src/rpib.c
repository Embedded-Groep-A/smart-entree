//RPI MET SOCKET NAAR WEMOS D1 MINI {BramsVoorhoofd}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pisocket.h"

#define PORT 8069
#define MAX_CLIENTS 4

typedef struct {
    int socket;
} Client;

Client clients[MAX_CLIENTS];

int main() {
    int server_fd = hostSocket(PORT);
    initClientArray();

    fd_set read_fds;
    int max_fd = server_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket > 0) {
                FD_SET(clients[i].socket, &read_fds);
                if (clients[i].socket > max_fd) {
                    max_fd = clients[i].socket;
                }
            }
        }

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            continue;
        }

        if (FD_ISSET(server_fd, &read_fds)) {
            int client_fd = acceptClient(server_fd);
            if (client_fd >= 0) {
                printf("New client connected\n");
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket > 0 && FD_ISSET(clients[i].socket, &read_fds)) {
                enum DataType type;
                int value;
                if (listenForData(clients[i].socket, &type, &value)) {
                    printf("Received data from client %d: Type: %d, Value: %d\n", i, type, value);
                } else {
                    printf("Client %d disconnected\n", i);
                    closeClient(clients[i].socket);
                    clients[i].socket = 0;
                }
            }
        }
    }

    closeSocket(server_fd);
    return 0;
}



