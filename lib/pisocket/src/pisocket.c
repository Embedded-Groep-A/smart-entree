#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include "pisocket.h"

int hostSocket(int port) {
    int server_fd;
    struct sockaddr_in address;
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
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    int flags = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

    printf("Client connected\n");

    return new_socket;
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
    struct hostent *server;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &address, sizeof(address));
    address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&address.sin_addr.s_addr, server->h_length);
    address.sin_port = htons(port);

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

void sendData(int socket, enum DataType type, int value) {
    char buffer[256];
    sprintf(buffer, "%d %d", type, value);
    send(socket, buffer, strlen(buffer), 0);
    printf("Data sent: %s\n", buffer);
}

int listenForData(int socket, enum DataType *type, int *value) {
    char buffer[256];
    int n = recv(socket, buffer, 255, 0);
    if (n <= 0) {
        return 0;
    }
    if (n > 0) {
        buffer[n] = '\0';
        sscanf(buffer, "%d %d", type, value);
        return 1;
    }
}
