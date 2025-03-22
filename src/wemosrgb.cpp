// RGB LED OP WEMOS D1 MINI
//  AANGESTUURD VIA SOCKET MET RPIB
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wmsocket.h"

#define RGB_R D5
#define RGB_G D6
#define RGB_B D7

const char *ssid = "707";
const char *password = "DEFCAB1960";
const char *hostname = "bramsvoorhoofd.local";
const int port = 8069;

WiFiClient client;

void setup()
{
    Serial.begin(115200);
    setupWiFiconnection(ssid, password);
    client = connectSocket(hostname, port);

    pinMode(RGB_R, OUTPUT);
    pinMode(RGB_G, OUTPUT);
    pinMode(RGB_B, OUTPUT);
}

void loop() {
    DataType type;
    uint8_t receivedData[4];
    size_t receivedSize;

    int readStatus = listenForData(client, &type, receivedData, &receivedSize);

    if (readStatus > 0) {
        Serial.print("Type: ");
        Serial.print(type);
        Serial.print(", Value: ");

        if (type == BUTTON || type == SENSOR) {
            int32_t value;
            memcpy(&value, receivedData, sizeof(value));
            Serial.println(value);
        } 
        else if (type == RGBLED) {
            uint8_t r = receivedData[0];
            uint8_t g = receivedData[1];
            uint8_t b = receivedData[2];

            Serial.printf("RGB: %u, %u, %u\n", r, g, b);

            analogWrite(RGB_R, r);
            analogWrite(RGB_G, g);
            analogWrite(RGB_B, b);
        }
    }
}

