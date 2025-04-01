// Gegenereerd door ChatGPT 03-mini-high met context de al bestaande code.
/*
 * Description:
 *   Integrated code for the Wemos device.
 *   - Establishes a WiFi connection using setupWiFiconnection().
 *   - Connects to a Raspberry Pi socket server via connectSocket().
 *   - Listens for incoming commands (e.g., BUTTON requests) from the Pi,
 *     executes the appropriate action (reading the button state), and sends the response.
 *   - Also monitors a physical button and sends an unsolicited message when pressed.
 */

#include <Arduino.h>
#include "wmsocket.h"

// WiFi network credentials
const char *ssid = "707";
const char *password = "DEFCAB1960";
const char *hostname = "bramsvoorhoofd.local";

// Raspberry pi ip
const char* piHost = "192.168.1.100";

WiFiClient client;

// Define the physical button pin.
#define BUTTON_PIN PA8

// Function to read the button state. Returns 1 when a press is detected.
int readButton() {
  static int lastState = HIGH;
  int currentState = digitalRead(BUTTON_PIN);
  int pressed = 0;
  if (lastState == HIGH && currentState == LOW) {
    pressed = 1; // Button press detected
  }
  lastState = currentState;
  return pressed;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Connect to WiFi
  setupWiFiconnection(ssid, password);

  // Connect to the Raspberry Pi server using the socket functions from wmsocket.h
  client = connectSocket(piHost, port);
}

void loop() {
  // Buffer to hold received data from the Raspberry Pi
  uint8_t buffer[10];
  size_t dataSize = 0;
  DataType receivedType;

  // Check for an incoming command from the Pi (non-blocking)
  int result = listenForData(client, &receivedType, buffer, &dataSize);
  if (result > 0) {
    // If a BUTTON command is received, read the button state and send it back
    if (receivedType == BUTTON) {
      int buttonState = readButton();
      sendToServer(client, BUTTON, &buttonState, sizeof(buttonState));
      Serial.print("Received BUTTON command, responded with state: ");
      Serial.println(buttonState);
    }
  }

  // Check the physical button for unsolicited presses
  if (readButton()) {
    int buttonState = 1;  // 1 indicates a button press
    sendToServer(client, BUTTON, &buttonState, sizeof(buttonState));
    Serial.println("Physical button pressed, message sent.");
    delay(300); // debounce delay
  }

  delay(50); // Main loop delay
}

