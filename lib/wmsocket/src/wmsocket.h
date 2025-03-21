#ifndef WMSOCKET_H
#define WMSOCKET_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

enum DataType
{
    BUTTON,
    SENSOR,
    RGBLED
};

void setupWiFiconnection(const char *ssid, const char *password);

WiFiClient connectSocket(const char *host, int port);

void sendToServer(WiFiClient &client, DataType type, int value);

int listenForData(WiFiClient &client, DataType *type, int *value);

#endif // WMSOCKET_H