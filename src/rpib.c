#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pisocket.h"

#define PORT 8069
#define MAX_CLIENTS 4

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
        printf("Activity: %d\n", activity);
        if (FD_ISSET(server_fd, &read_fds)) {
            int client_fd = acceptClient(server_fd);

            if (client_fd >= 0) {
                int client_id = assignClientId();
                if (client_id == -1) {
                    printf("Max clients reached. Rejecting new client.\n");
                    close(client_fd);
                } else {
                    clients[client_id].socket = client_fd;
                    clients[client_id].id = client_id;
                    printf("New client connected, assigned ID: %d\n", client_id);
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket > 0 && FD_ISSET(clients[i].socket, &read_fds)) {
                enum DataType type;
                int value;
                int readStatus = listenForData(clients[i].socket, &type, &value);

                if (readStatus > 0) {
                    printf("Received data from client %d: Type: %d, Value: %d\n", clients[i].id, type, value);
                } else if (readStatus == 0) {
                    printf("Client %d disconnected\n", clients[i].id);
                    closeClient(clients[i].socket);
                    clients[i].socket = 0;
                    clients[i].id = -1;
                }
            }
        }
}
