#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wmsocket.h"

void setupWiFiconnection(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi.");
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.println(WiFi.localIP());
}

WiFiClient connectSocket(const char* host, int port) {
    WiFiClient client;
    IPAddress serverip;
    WiFi.hostByName(host, serverip);
    if (client.connect(serverip, port)) {
        Serial.println("Connected to " + String(host) + " on port " + String(port));
      }
      return client;
}

void sendToServer(WiFiClient &client, DataType type, void *value, size_t size) {
    uint8_t buffer[1 + size];
    buffer[0] = (uint8_t)type;
    memcpy(buffer + 1, value, size);

    client.write(buffer, sizeof(buffer));
    Serial.printf("Data sent (Type: %d, Size: %zu bytes)\n", type, size);
}

int listenForData(WiFiClient &client, DataType *type, void *value, size_t *size) {
    if (!client.connected() || !client.available()) {
        return 0;
    }

    uint8_t buffer[4];

    int bytesRead = client.read(buffer, sizeof(buffer));
    if (bytesRead < 1) return 0;

    *type = (DataType)buffer[0];

    switch (*type) {
        case BUTTON:
        case SENSOR:
            if (bytesRead < 5) return -1;
            memcpy(value, buffer + 1, sizeof(int32_t));
            *size = sizeof(int32_t);
            break;
        case RGBLED:
            if (bytesRead < 4) return -1;
            memcpy(value, buffer + 1, 3);
            *size = 3;
            break;
        default:
            return -1;
    }
    return 1;
}