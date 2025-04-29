  #include <SPI.h>
  #include <MFRC522.h>
  #include <Servo.h>
  #include <SoftwareSerial.h>

  // RFID
  #define RST_PIN 9
  #define SS_PIN 10
  MFRC522 mfrc522(SS_PIN, RST_PIN);

  // Deurbel knop
  #define DEURBEL_PIN 2

  // Servo
  Servo deurServo;
  #define SERVO_PIN 5

  // RS485 via SoftwareSerial
  #define RS485_RX 6  
  #define RS485_TX 7  
  SoftwareSerial rs485Serial(RS485_RX, RS485_TX); // RX, TX

  String wachtOpAntwoord(unsigned long timeout_ms = 50000) {
    unsigned long starttijd = millis();
    String antwoord = "";

    while (millis() - starttijd < timeout_ms) {
      if (rs485Serial.available()) {
        antwoord = rs485Serial.readStringUntil('\n');
        antwoord.trim();
        break;
      }
    }
    return antwoord;
  }

  void openDeur() {
    deurServo.write(90);  // Deur openen
    delay(30000);          // 30 seconden open
    deurServo.write(0);   // Deur sluiten
  }

  void setup() {
    pinMode(DEURBEL_PIN, INPUT_PULLUP);

    deurServo.attach(SERVO_PIN);
    deurServo.write(0); // Start gesloten

    Serial.begin(115200);        // USB-debug
    rs485Serial.begin(115200);   // RS485 communicatie via SoftwareSerial
    SPI.begin();
    mfrc522.PCD_Init();

    Serial.println("Systeem opgestart...");
  }

  void loop() {
    // 1. RFID-kaart scannen
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String uidString = "UID:";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
        uidString += String(mfrc522.uid.uidByte[i], HEX);
      }

      Serial.print("Kaart gescand, UID: ");
      Serial.println(uidString);

      rs485Serial.println(uidString);  // Stuur naar Pi

      Serial.println("Wacht op antwoord van Pi...");
      String antwoord = wachtOpAntwoord();
      if (antwoord == "ACCEPT") {
        Serial.println(" Toegang geaccepteerd, deur openen...");
        openDeur();
      } else if (antwoord == "REJECT") {
        Serial.println(" Toegang geweigerd door Pi.");
      } else {
        Serial.println(" Geen geldig antwoord ontvangen na UID.");
      }

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      delay(1000);
    }

    if (digitalRead(DEURBEL_PIN) == LOW) {
      Serial.println("🔔 Deurbel ingedrukt, stuur BEL:IN naar Pi...");
      rs485Serial.println("BEL:IN");

      Serial.println("Wacht op antwoord van Pi voor deurbel...");
      String antwoord = wachtOpAntwoord();
      if (antwoord == "OPEN") {
        Serial.println(" Pi gaf OPEN, deur openen...");
        openDeur();
      } else {
        Serial.println(" Geen OPEN ontvangen na deurbel.");
      }

      delay(500); // debounce
    }
  }
