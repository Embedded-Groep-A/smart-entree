#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Deurbel knop
#define DEURBEL_PIN 3

// Servo
Servo deurServo;
#define SERVO_PIN 5

String wachtOpAntwoord(unsigned long timeout_ms = 5000) {
  unsigned long starttijd = millis();
  String antwoord = "";

  while (millis() - starttijd < timeout_ms) {
    if (Serial.available()) { // Let op: Serial is nu pin 0/1
      antwoord = Serial.readStringUntil('\n');
      antwoord.trim();
      break;
    }
  }
  return antwoord;
}

void openDeur() {
  deurServo.write(90);  // Deur openen
  delay(3000);          // 3 seconden open
  deurServo.write(0);   // Deur sluiten
}

void setup() {
  pinMode(DEURBEL_PIN, INPUT_PULLUP);

  deurServo.attach(SERVO_PIN);
  deurServo.write(0); // Deur begint gesloten

  Serial.begin(115200); 
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // 1. RFID-kaart scannen
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidString = "UID:";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
      uidString += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.println(uidString);  // Stuur UID naar de Pi via RS485

    String antwoord = wachtOpAntwoord();
    if (antwoord == "ACCEPT") {
      openDeur();
    }
    // Bij REJECT of geen antwoord: niks doen

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }

  // 2. Deurbelknop
  if (digitalRead(DEURBEL_PIN) == LOW) {
    Serial.println("BEL:IN");  // Stuur naar Pi

    String antwoord = wachtOpAntwoord();
    if (antwoord == "OPEN") {
      openDeur();
    }

    delay(500); // debounce
  }
}
