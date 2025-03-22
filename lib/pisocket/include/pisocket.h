#ifndef PISOCKET_H
#define PISOCKET_H

#define MAX_CLIENTS 4
#define BUFFER_SIZE 256

typedef struct {
    int socket;
    int id;
} Client;

extern Client clients[MAX_CLIENTS];

enum DataType {
    BUTTON,
    SENSOR,
    RGBLED
};

void initClientArray();

int assignClientId();


int hostSocket(int port);

int acceptClient(int server);

void closeClient(int client);

void closeSocket(int server);

int connectSocket(char *host, int port);

void disconnectSocket(int server);

void sendToServer(int socket, enum DataType type, int value);

void sendToClient(int clientId, enum DataType type, void *value, size_t size);

int listenForData(int socket, enum DataType *type, void *value, size_t *size);

#endif // PISOCKET_H