#include <Wire.h>

#define I2C_ADDR 0x08

void setup() {
    Wire.begin(I2C_ADDR);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    Serial.begin(115200);
    Serial.println("STM32 I2C Slave Ready.");
}

volatile uint8_t receivedCommand = 0;

void receiveEvent(int bytes) {
    if (Wire.available()) {
        receivedCommand = Wire.read();
        Serial.print("Received command: ");
        Serial.println(receivedCommand);
    }
}

void requestEvent() {
    uint8_t dataToSend = receivedCommand + 10;  // Example response
    Wire.write(dataToSend);
    Serial.print("Sent response: ");
    Serial.println(dataToSend);
}

void loop() {
    // Do other processing if needed
}
