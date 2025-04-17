#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Voeg hier je bekende kaarten toe (4 bytes per kaart)
byte knownUIDs[3][4] = {
  {0x64, 0x81, 0xE6, 0x03},  // Kaart 1
  {0xB1, 0xFF, 0x74, 0x1D},  // Kaart 2
  // {0x11, 0x22, 0x33, 0x44}   // Kaart 3
};
const char* eigenaarNamen[3] = {
  "Cas",
  "Thijs",
  // "Ahmed"
};
int checkUID(byte *uid) {
  for (int i = 0; i < 3; i++) {  // Aantal bekende kaarten
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (uid[j] != knownUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return i;  // Index van match
  }
  return -1;  // Geen match
}


void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Systeem opgestart...");
  Serial.println("Scan een RFID-kaart...");
}

void loop() {
  // Wacht op een kaart
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID
  Serial.print("UID tag: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Controleer of UID voorkomt in de lijst
 int uidIndex = checkUID(mfrc522.uid.uidByte);
if (uidIndex >= 0) {
  Serial.print("✅ Welkom ");
  Serial.print(eigenaarNamen[uidIndex]);
  Serial.println("!");
} else {
  Serial.println("⛔ Onbekende kaart. Toegang geweigerd.");
}

  // Reset kaartlezing en wacht op volgende
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(1500);

  Serial.println("Scan een nieuwe RFID-kaart...");
}

