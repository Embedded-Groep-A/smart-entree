//KNOP OP STM32L432KC
//VERBONDEN MET RPIA VIA I2C
#include <Arduino.h>
#include <Wire.h>

#define ADDR 0x69

#define BTN PB5

int readButton(int button) {
    static int pressed = 0;

    int reading = digitalRead(button);

    if (reading == LOW && pressed == 0) {
        pressed = 1;
        return 1;
    } else if (reading == HIGH) {
        pressed = 0;
    }

    return 0;
}

void setup() {
    Serial.begin(115200);
    Wire.begin(ADDR);

    pinMode(BTN, INPUT_PULLUP);
}


void loop() {
    if (readButton(BTN)) {
        Serial.println("Button pressed");
        Wire.beginTransmission(ADDR);
        Wire.write("BUTTON", 6);
        Wire.endTransmission();
    }
    
}