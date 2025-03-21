/**
 * @file pisocket.h
 * @brief Header file van de pisocket library.
 */
#ifndef PISOCKET_H
#define PISOCKET_H

#define MAX_CLIENTS 4 /**<Maximaal aantal clients dat kan worden geaccepteerd.*/
#define BUFFER_SIZE 256 /**<Grootte van de buffer voor het ontvangen van data.*/

typedef struct {
    int socket;
    int id;
} Client; /**<Structuur voor het bijhouden van client sockets.*/

extern Client clients[MAX_CLIENTS];


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
 * @brief Initialiseert de client array.
 */
void initClientArray();

/**
 * @brief Wijs een client ID toe.
 * @return De eerstvolgende vrije client ID of -1 als vol.
 */
int assignClientId();


/**
 * @brief Host een server socket op de opgegeven poort.
 *
 * @param port Poortnummer waarop de server socket wordt gehost.
 * @return De file descriptor van de server socket.
 */
int hostSocket(int port);

/**
 * @brief Accepteert een client socket op de opgegeven server socket.
 *
 * @param server De file descriptor van de server socket.
 * @return De file descriptor van de geaccepteerde client socket.
 */
int acceptClient(int server);

/**
 * @brief Sluit de opgegeven client socket.
 *
 * @param client De file descriptor van de client socket die gesloten wordt.
 */
void closeClient(int client);

/**
 * @brief Sluit de opgegeven server socket.
 *
 * @param server De file descriptor van de server socket die gesloten wordt.
 */
void closeSocket(int server);

/**
 * @brief Verbindt met een server op de opgegeven host en poort.
 * @param host De hostnaam van de server.
 * @param port Het poortnummer van de server.
 * @return De file descriptor van de client socket.
 */ 
int connectSocket(char *host, int port);

/**
 * @brief verbreekt de verbinding met de server.
 * @param server De file descriptor van de server socket.
 */
void disconnectSocket(int server);



/**
 * @brief Verzendt data van het opgegeven type en waarde.
 *
 * @param socket De file descriptor van de socket
 * @param type De type van de data die wordt verzonden.
 * @param value De waarde van de data die wordt verzonden.
 */
void sendToServer(int socket, enum DataType type, int value);

/**
 * @brief Verzendt data van het opgegeven type en waarde.
 *
 * @param clientId De id van de client
 * @param type De type van de data die wordt verzonden.
 * @param value De waarde van de data die wordt verzonden.
 */
void sendToClient(int clientId, enum DataType type, int value);

/**
 * @brief Luistert naar data die wordt ontvangen.
 * @param socket De file descriptor van de socket.
 * @param type De pointer naar de variabele waarin de ontvangen type wordt opgeslagen.
 * @param value De pointer naar de variabele waarin de ontvangen waarde wordt opgeslagen.
 * @return 1 als er data is ontvangen, anders 0.
 */
int listenForData(int socket, enum DataType *type, int *value);

#endif // PISOCKET_H