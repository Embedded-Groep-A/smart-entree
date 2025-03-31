#include <Arduino.h>
#include <HardwareSerial.h>  // Ensure correct inclusion for STM32

#define USART_TX PIN_SERIAL2_TX
#define USART_RX PIN_SERIAL2_RX

void setup() {
    Serial.begin(115200);        // Initialize Serial Monitor
    Serial.println("STM32 Ready");
}

void loop() {
    // Check for data from Serial Monitor
    if (Serial.available()) {
        String received = Serial.readString();
        Serial.print("Received on Serial: ");
        Serial.println(received);

    }

    // Check for data from USART2
    if (Serial.available()) {
        String received = Serial.readString();
        Serial.print("Received on USART2: ");
        Serial.println(received);
    }

    delay(1000);
}
