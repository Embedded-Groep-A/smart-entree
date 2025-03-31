// STM32 Arduino Code (STM32_USART_Communication.ino)

#include <Arduino.h>

#define USART_TX PA2
#define USART_RX PA3

void setup() {
    Serial.begin(115200);  // Initialize USART2
    pinMode(USART_TX, OUTPUT);
    pinMode(USART_RX, INPUT);
    delay(1000);
    Serial.println("STM32 Ready");
}

void loop() {
    if (Serial.available()) {
        String received = Serial.readString();
        Serial.print("Received: ");
        Serial.println(received);
        Serial.println("Reply from STM32");
        delay(500);
    }
    Serial.println("Hello from STM32!");
    delay(1000);
}
