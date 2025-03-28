#include <Arduino.h>

#define TX_PIN PA2
#define RX_PIN PA3
#define DE_PIN PA4
#define RE_PIN PA5

// Explicitly define Serial1 on PA2 (TX) and PA3 (RX)
HardwareSerial Serial1(PA2, PA3);

void enableTransmit() {
    digitalWrite(DE_PIN, HIGH);
    digitalWrite(RE_PIN, HIGH);
}

void enableReceive() {
    digitalWrite(DE_PIN, LOW);
    digitalWrite(RE_PIN, LOW);
}

void setup() {
    Serial1.begin(9600);  // Initialize UART2 (PA2, PA3) for RS485
    Serial.begin(9600);   // USB Serial for debugging
    pinMode(DE_PIN, OUTPUT);
    pinMode(RE_PIN, OUTPUT);
    enableReceive();  // Start in receive mode
}

void loop() {
    // Check for incoming data
    if (Serial1.available() > 0) {
        String received = Serial1.readString();
        Serial.println("Received from RPi: " + received);

        // Respond back
        enableTransmit();
        delay(1);
        Serial1.println("Hello RPi!");
        delay(1);
        enableReceive();
    }
    delay(100);
}
