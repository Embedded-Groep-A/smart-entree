#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <string.h>

#define DE_PIN 18    // GPIO18 for DE (Driver Enable)
#define RE_PIN 23    // GPIO23 for RE (Receiver Enable)
#define UART_DEVICE "/dev/serial0"  // UART device on Raspberry Pi

void enableTransmit() {
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
}

void enableReceive() {
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
}

int main() {
    int serial_port;
    char buffer[100];

    // Initialize wiringPi
    if (wiringPiSetupGpio() == -1) {
        perror("WiringPi setup failed");
        return 1;
    }

    // Set up GPIO pins
    pinMode(DE_PIN, OUTPUT);
    pinMode(RE_PIN, OUTPUT);
    enableReceive();

    // Open serial port
    if ((serial_port = serialOpen(UART_DEVICE, 9600)) < 0) {
        perror("Unable to open serial device");
        return 1;
    }

    while (1) {
        // Send data
        enableTransmit();
        serialPuts(serial_port, "Hello STM32!\n");
        serialFlush(serial_port);
        usleep(10000);  // Small delay
        enableReceive();

        // Read response
        usleep(100000);  // Wait for response
        if (serialDataAvail(serial_port)) {
            int len = 0;
            while (serialDataAvail(serial_port)) {
                buffer[len++] = serialGetchar(serial_port);
                if (len >= sizeof(buffer) - 1) break;
            }
            buffer[len] = '\0';
            printf("Received from STM32: %s", buffer);
        }
        
        sleep(1);
    }

    serialClose(serial_port);
    return 0;
}
