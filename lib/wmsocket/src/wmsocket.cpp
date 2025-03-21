#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wmsocket.h"

void setupWiFiconnection(char *ssid, char *password) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi.");
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.println(WiFi.localIP());
}

WiFiClient connectSocket(char *host, int port) {
    WiFiClient client;
    IPAddress serverip;
    WiFi.hostByName(host, serverip);
    if (client.connect(serverip, port)) {
        Serial.println("Connected to " + String(host) + " on port " + String(port));
      }
      return client;
}

void sendData(WiFiClient &client, DataType type, int value) {
    char buffer[32];
    sprintf(buffer, "%d %d", type, value);
    client.print(buffer);
    client.print("\n");
    Serial.printf("Data sent: %s\n", buffer);
}

int listenForData(WiFiClient &client, DataType *type, int *value) {
    if (client.available()) {
        char buffer[32];
        client.readBytesUntil('\n', buffer, 32);
        int typeInt;
        sscanf(buffer, "%d %d", &typeInt, value);
        *type = (enum DataType)typeInt;
        return 1;
    }
    return 0;
}