#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringSerial.h>


const char *serial_port = "/dev/ttyS0";
int baud_rate = 115200;

char readLine(int fd, char *buffer, int size) {
    int index = 0;
    while (index < size - 1) {
        if (serialDataAvail(fd)) {
            char c = serialGetchar(fd);
            if (c == '\n') {
                break;
            } else {
                buffer[index++] = c;
            }
        }
    }
    buffer[index] = '\0';
    return index;
}

typedef struct {
    uint8_t uid[4];
    const char* eigenaarNaam;
} Eigenaar;

Eigenaar eigenaars[] = {
    {{0x77, 0xDA, 0xAC, 0x02}, "Cas"},
    {{0xB1, 0xFF, 0x74, 0x1D}, "Thijs"},
    {{0x64, 0x81, 0xE6, 0x03}, "Ahmed"}
};

int checkUID(uint8_t *uid) {
    for (int i = 0; i < (sizeof(eigenaars) / sizeof(eigenaars[0])); i++) {
        int match = 1;
        for (int j = 0; j < 4; j++) {
            if (uid[j] != eigenaars[i].uid[j]) {
                match = 0;
                break;
            }
        }
        if (match) return i;  // Return index of matched UID
    }
    return -1;
}


int main() {
    printf("BLIEP BLOEP MAINFRAME BUS ONLINE RPIA\n");
    int fd = serialOpen(serial_port, baud_rate);

    char buffer[256];
    int index = 0;

    while (1) {
        serialPuts(fd, "OPEN\n");
        serialFlush(fd);
        usleep(10000); // 1 second delay

        // if (serialDataAvail(fd)) {
        //     index = readLine(fd, buffer, sizeof(buffer));
        //     if (index > 0) {
        //         if (strncmp(buffer, "UID:", 4) == 0) {
        //             uint8_t uid[4];
        //             sscanf(buffer + 4, "%hhx %hhx %hhx %hhx", &uid[0], &uid[1], &uid[2], &uid[3]);
        //             printf("UID: %02X %02X %02X %02X\n", uid[0], uid[1], uid[2], uid[3]);
        //             int eigenaarIndex = checkUID(uid);
        //             if (eigenaarIndex != -1) {
        //                 printf("Eigenaar: %s\n", eigenaars[eigenaarIndex].eigenaarNaam);
        //                 serialPuts(fd, "OPEN\n");
        //                 serialFlush(fd);
        //             } else {
        //                 printf("Onbekende UID\n");
        //                 serialPuts(fd, "WEIGER\n");
        //                 serialFlush(fd);
        //             }
        //         }
                
        //     }

        // }

    }



    return 0;
}