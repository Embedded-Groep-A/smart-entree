#include <stdio.h>
#include <stdlib.h>
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

    printf("Client connected\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == 0) {
            clients[i].socket = new_socket;
            clients[i].id = i;
            printf("Assigned client ID: %d\n", i);
            return new_socket;
        }
    }

    printf("Max clients on server\n");
    close(new_socket);
    return -1;
}

void closeClient(int client) {
    close(client);
    printf("Client disconnected\n");
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

void sendToServer(int socket, enum DataType type, int value) {
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%d %d", type, value);
    write(socket, buffer, strlen(buffer));
    printf("Data sent: %s\n", buffer);
}

void sendToClient(int clientId, enum DataType type, int value) {
    if (clientId < 0 || clientId >= MAX_CLIENTS || clients[clientId].socket == 0) {
        printf("Client %d not connected\n", clientId);
        return;
    }

    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%d %d", type, value);
    write(clients[clientId].socket, buffer, strlen(buffer));
    printf("Data sent to client %d: %s\n", clientId, buffer);
}

int listenForData(int socket, enum DataType *type, int *value) {
    char buffer[BUFFER_SIZE];
    int n = read(socket, buffer, 255);
    printf("Read status: %d\n", n);
    if (n <= 0) {
        return 0;
    }

    buffer[n] = '\0';
    int typeInt;
    sscanf(buffer, "%d %d", &typeInt, value);
    *type = (enum DataType)typeInt;

    return 1;
}
