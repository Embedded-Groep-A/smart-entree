#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include "pisocket.h"

struct sockaddr_in address;

Client clients[MAX_CLIENTS] = {0};

void initClientArray() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = 0;
        clients[i].id = -1;
    }
}

int assignClientId() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == 0) {
            return i;
        }
    }
    return -1;
}

int hostSocket(int port) {
    int server_fd;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 10);

    printf("Server listening on port %d\n", port);

    return server_fd;
}

int acceptClient(int server) {
    int addrlen = sizeof(address);
    int new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    int flags = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

    int client_id = assignClientId();
    if (client_id >= 0) {
        clients[client_id].socket = new_socket;
        clients[client_id].id = client_id;
        printf("New client connected, assigned ID: %d\n", client_id);
        return new_socket;
    } else {
        printf("Max clients reached. Rejecting new client.\n");
        close(new_socket);
        return -1;
    }
}

void closeClient(int client) {
    close(client);
}

void closeSocket(int server) {
    close(server);
    printf("Server closed\n");
}

int connectSocket(char *host, int port) {
    int client_fd;
    struct sockaddr_in address;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    struct hostent *server = gethostbyname(host);
    memcpy(&address.sin_addr.s_addr, server->h_addr, server->h_length);

    connect(client_fd, (struct sockaddr *)&address, sizeof(address));

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    printf("Connected to server at %s:%d\n", ip, port);

    return client_fd;
}

void disconnectSocket(int server) {
    close(server);
    printf("Disconnected from server\n");
}

void sendToServer(int socket, enum DataType type, void *value, size_t size) {
    uint8_t buffer[BUFFER_SIZE];
    buffer[0] = (uint8_t)type;
    memcpy(buffer + 1, value, size);
    write(socket, buffer, size + 1);
    printf("Data sent to server %d (Type: %d, Size: %zu bytes)\n", type, size);
}

void sendToClient(int clientId, enum DataType type, void *value, size_t size) {
    if (clientId < 0 || clientId >= MAX_CLIENTS || clients[clientId].socket == 0) {
        printf("Client %d not connected\n", clientId);
        return;
    }

    uint8_t buffer[BUFFER_SIZE];
    buffer[0] = (uint8_t)type;
    memcpy(buffer + 1, value, size);
    write(clients[clientId].socket, buffer, size + 1);
    printf("Data sent to client %d (Type: %d, Size: %zu bytes)\n", clientId, type, size);
}

int listenForData(int socket, enum DataType *type, void *value, size_t *size) {
    uint8_t buffer[BUFFER_SIZE];
    int n = read(socket, buffer, BUFFER_SIZE);

    if (n <= 0) {
        return n;
    }

    *type = (enum DataType)buffer[0];

    switch (*type) {
        case BUTTON:
        case SENSOR:
            if (n < 5) return -1;
            memcpy(value, buffer + 1, sizeof(int32_t));
            *size = sizeof(int32_t);
            break;
        case RGBLED:
            if (n < 4) return -1;
            memcpy(value, buffer + 1, 3);
            *size = 3;
            break;
        default:
            return -1;
    }
    return 1;
}
