#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial KappaSerial(USART1);

#define BTN PA8

int readButton() {
    static bool lastState = HIGH;
    bool currentState = digitalRead(BTN);

    if (lastState == HIGH && currentState == LOW) {
        lastState = currentState;
        return 1; // Button pressed
    }

    lastState = currentState;
    return 0; // No press
}

void setup() {
    Serial.begin(115200);
    KappaSerial.begin(115200);
    pinMode(BTN, INPUT_PULLUP);
}

void loop() {
    if (readButton()) {
        Serial.println("Button pressed!");
        KappaSerial.println("BTN");
    }
    delay(50);
}
