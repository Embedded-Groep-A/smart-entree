//RPI MET SOCKET NAAR WEMOS D1 MINI {BramsVoorhoofd}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pisocket.h"




#define PORT 8080



int main() {
    int server_fd = hostSocket(PORT);
    while (1) {
        //int client_fd = acceptClient(server_fd);
        enum DataType type;
        int value;
        if (listenForData(client_fd, &type, &value) > 0) {
            printf("Data received:\nType: %d, Value: %d\n", type, value);
        }
    }
    return 0;
}



