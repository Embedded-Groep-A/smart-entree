
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "define.h"

// RFID
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Deurbel knop
#define DEURBEL_PIN 2

// Servo
Servo deurServo;
#define SERVO_PIN 5

void openDeur() {
  deurServo.write(90);  // Deur openen
  delay(30000);         // 30 seconden open
  deurServo.write(0);   // Deur sluiten
}

void setup() {
  pinMode(DEURBEL_PIN, INPUT_PULLUP);

  deurServo.attach(SERVO_PIN);
  deurServo.write(0); // Start gesloten

  Serial.begin(115200);        // USB-debug
  Serial1.begin(9600);         // RS485 via hardware Serial1 (pins 0/1)

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Systeem opgestart...");
}

void loop() {
  // Lees binnenkomende RS485-data
  if (Serial1.available()) {
    char incomingByte = Serial1.read();  
    Serial.write(incomingByte);  // Print naar de USB monitor
  }
  // Check deurbel
  if (digitalRead(DEURBEL_PIN) == LOW) {
    Serial.println("Deurbel ingedrukt, stuur BEL:IN naar Pi...");
    Serial1.println("BEL:IN");

    Serial.println("Wacht op antwoord van Pi voor deurbel...");
    String antwoord = "";
    unsigned long start = millis();
    while (millis() - start < 2000) {
      if (Serial1.available()) {
        char c = Serial1.read();
        antwoord += c;
      }
    }
    antwoord.trim();

    Serial.print("Ontvangen antwoord: ");
    Serial.println(antwoord);

    if (antwoord == "OPEN") {
      Serial.println("Antwoord is OPEN, deur openen...");
      openDeur();
    } else {
      Serial.println("Geen geldig antwoord ontvangen voor deurbel.");
    }

    delay(500); // debounce
  }
  // Check RFID-kaart
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidString = "UID:";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
        uidString += String(mfrc522.uid.uidByte[i], HEX);
        uidString += " ";
    }
    uidString.trim(); 

    Serial.print("Kaart gescand, UID: ");
    Serial.println(uidString);
    Serial1.println(uidString);  // Stuur naar Pi

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }


}
