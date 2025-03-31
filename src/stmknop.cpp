#include <Arduino.h>
#include <HardwareSerial.h>  // Ensure correct inclusion for STM32

#define USART_TX PIN_SERIAL2_TX
#define USART_RX PIN_SERIAL2_RX

void setup() {
    Serial.begin(115200);        // Initialize Serial Monitor
    Serial2.begin(9600);         // Initialize USART2
    Serial.println("STM32 Ready");
    Serial2.println("USART2 Ready");
}

void loop() {
    // Check for data from Serial Monitor
    if (Serial.available()) {
        String received = Serial.readString();
        Serial.print("Received on Serial: ");
        Serial.println(received);
        Serial2.print("Forwarded to USART2: ");
        Serial2.println(received);  // Forward to USART2
    }

    // Check for data from USART2
    if (Serial2.available()) {
        String received = Serial2.readString();
        Serial.print("Received on USART2: ");
        Serial.println(received);
    }

    // Periodic message
    Serial.println("Hello from STM32!");
    Serial2.println("Hello from STM32 on USART2!");
    delay(1000);
}
