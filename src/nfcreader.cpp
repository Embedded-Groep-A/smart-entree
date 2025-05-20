
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "define.h"

// RFID
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Pins
#define DEURBEL_PIN 2           // Deurbel knop
#define BINNENKANT_PIN 6        // Binnenknop
#define SERVOVOOR_PIN 5             // Servo 1 (voorste deur)
#define SERVOACHTER_PIN 3            // Servo 2 (achterste deur)

// Servo's
Servo deurServovoor;     // Voorste deur
Servo deurServoAchter;    // Achterste deur

void openDeur() {
  Serial.println("Deur openen...");
  deurServovoor.write(180);
  delay (3000);
  deurServoAchter.write(180);
  deurServovoor.write(0);
  deurServoAchter.write(0);
  Serial.println("Deur gesloten.");
}
void openDeurVanBinnen() {
  deurServoAchter.write(180);
  Serial.println("Achterdeur geopend.");
  delay(3000);  // tijd om naar voordeur te lopen

  deurServovoor.write(180);
  Serial.println("Voorste deur geopend.");
  delay(5000); // resterende tijd

  deurServoAchter.write(0);
  deurServovoor.write(0);
  Serial.println("Beide deuren gesloten.");
}

String leesUID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidString = "UID:";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
      uidString += String(mfrc522.uid.uidByte[i], HEX);
      uidString += " ";
    }
    uidString.trim();
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return uidString;
  }
  return "";
}

String wachtOpRS485Antwoord(unsigned long timeout) {
  String antwoord = "";
  unsigned long start = millis();
  while (millis() - start < timeout) {
    if (Serial1.available()) {
      char c = Serial1.read();
      antwoord += c;
    }
  }
  antwoord.trim();
  return antwoord;
}

void verwerkDeurbel() {
  Serial.println("Deurbel ingedrukt, stuur BEL:IN naar Pi...");
  
  Serial1.println("@");//BEL:IN

  Serial.println("Wacht op antwoord van Pi voor deurbel (5s)...");
  unsigned long start = millis();

  while (millis() - start < 5000) {
    String uid = leesUID();
    if (!uid.isEmpty()) {
      Serial.println("Pas aangeboden tijdens deurbel, override logica...");
      Serial.print("Kaart UID: ");
      Serial.println(uid);
      Serial1.println(uid);
      return;
    }
  }

  String antwoord = wachtOpRS485Antwoord(5000); // Kort extra moment om te checken
  Serial.print("Ontvangen antwoord: ");
  Serial.println(antwoord);
  if (antwoord == "OPEN") {//OPEN
    Serial.println("Antwoord is OPEN, deur openen...");
    openDeur();
  } else {
    Serial.println("Geen geldig antwoord ontvangen voor deurbel.");
  }
}

void setup() {
  pinMode(DEURBEL_PIN, INPUT_PULLUP);
  pinMode(BINNENKANT_PIN, INPUT_PULLUP);

  deurServovoor.attach(SERVOVOOR_PIN);
  deurServoAchter.attach(SERVOACHTER_PIN);
  deurServovoor.write(0);
  deurServoAchter.write(0);

  Serial.begin(115200);
  Serial1.begin(9600);

  Serial.println("RFID Init gestart...");
  mfrc522.PCD_Init();
  Serial.println("RFID Init klaar.");
  Serial.println("Systeem opgestart...");
}

void loop() {
  // Lees RS485
  if (Serial1.available()) {
    String antwoord = "";
    while (Serial1.available()) {
      antwoord += (char)Serial1.read();
    }
    antwoord.trim();
    Serial.print("Ontvangen via RS485: ");
    Serial.println(antwoord);
    if (antwoord == "OPEN") {
      openDeur();
    }
  }

  // Lees RFID
  String uid = leesUID();
  if (!uid.isEmpty()) {
    Serial.print("Kaart gescand, UID: ");
    Serial.println(uid);
    Serial1.println(uid);
    delay(500);  // debounce
    return;
  }

  // Deurbel indrukken
  if (digitalRead(DEURBEL_PIN) == LOW) {
    verwerkDeurbel();
    delay(500); // debounce
  }

  // Binnenknop indrukken
  if (digitalRead(BINNENKANT_PIN) == LOW) {
    Serial.println("Binnenknop ingedrukt.");
    openDeurVanBinnen();
    delay(500); // debounce
  }
}
