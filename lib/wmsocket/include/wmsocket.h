/**
 * @file wmsocket.h
 * @brief Header file for WeMos socket
 */

#ifndef WMSOCKET_H
#define WMSOCKET_H

#include <Arduino.h>
#include <ESP8266WiFi.h>


/**
 * @brief Verbindt de WeMos aan een WiFi netwerk
 *  * 
 * @param ssid SSID van het WiFi netwerk.
 * @param password Wachtwoord van het WiFi netwerk.
 */
void setupWiFiconnection(char *ssid, char *password);

/**
 * @brief Verbindt met een server op de opgegeven host en poort.
 * @param host De hostnaam van de server.
 * @param port Het poortnummer van de server.
 * @return WiFiClient
 */
WiFiClient connectSocket(char *host, int port);

/**
 * @enum DataType
 * @brief Enumeratie van de verschillende types van data die kunnen worden verzonden.
 */
enum DataType {
    BUTTON, /**<Button data>*/
    SENSOR, /**<Semsor data>*/
    RGBLED /**<RGB LED data>*/
};

/**
 * @brief Verzendt data van het opgegeven type en waarde.
 *
 * @param socket De file descriptor van de socket
 * @param type De type van de data die wordt verzonden.
 * @param value De waarde van de data die wordt verzonden.
 */
void sendData(int socket, enum DataType type, int value);

/**
 * @brief Luistert naar data die wordt ontvangen.
 * @param socket De file descriptor van de socket.
 * @param type De pointer naar de variabele waarin de ontvangen type wordt opgeslagen.
 * @param value De pointer naar de variabele waarin de ontvangen waarde wordt opgeslagen.
 * @return 1 als er data is ontvangen, anders 0.
 */
int listenForData(int socket, enum DataType *type, int *value);

#endif // WMSOCKET_H