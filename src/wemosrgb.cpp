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

void loop()
{
    enum DataType type;
    int value;
    int readStatus = listenForData(&client, &type, &value);
    Serial.println("type: " + String(type) + ", value: " + String(value));

    if (readStatus > 0)
    {
        if (type == RGBLED)
        {
            analogWrite(RGB_R, (value >> 16) & 0xFF);
            analogWrite(RGB_G, (value >> 8) & 0xFF);
            analogWrite(RGB_B, value & 0xFF);
        }
    }
}
