#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "pisocket.h"

struct sockaddr_in address;

int hostSocket(int port) {
    int server_fd;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    return server_fd;
}

int acceptClient(int server) {
    int addrlen = sizeof(address);
    int new_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept failed");
        return -1;
    }

    printf("Client connected\n");

    return new_socket;
}

void closeClient(int client) {
    if (close(client) < 0) {
        perror("close client failed");
    } else {
        printf("Client disconnected\n");
    }
}

void closeSocket(int server) {
    if (close(server) < 0) {
        perror("close server failed");
    } else {
        printf("Server closed\n");
    }
}

int connectSocket(char *host, int port) {
    int client_fd;
    struct sockaddr_in address;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "gethostbyname failed\n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    memcpy(&address.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    printf("Connected to server at %s:%d\n", ip, port);

    return client_fd;
}

void disconnectSocket(int server) {
    if (close(server) < 0) {
        perror("disconnect failed");
    } else {
        printf("Disconnected from server\n");
    }
}

void sendData(int socket, enum DataType type, int value) {
    char buffer[256];
    sprintf(buffer, "%d %d", type, value);
    if (write(socket, buffer, strlen(buffer)) < 0) {
        perror("send data failed");
    } else {
        printf("Data sent: %s\n", buffer);
    }
}

int listenForData(int socket, enum DataType *type, int *value) {
    char buffer[256];
    int n = read(socket, buffer, 255);
    if (n < 0) {
        perror("read failed");
        return 0;
    } else if (n == 0) {
        return 0;
    } else {
        buffer[n] = '\0';
        if (sscanf(buffer, "%d %d", (int *)type, value) != 2) {
            fprintf(stderr, "sscanf failed\n");
            return 0;
        }
        return 1;
    }
}
