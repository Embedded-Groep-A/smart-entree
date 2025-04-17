#ifndef NFCREADER_H
#define NFCREADER_H

#include <MFRC522.h>

// Bekende UIDs
extern byte knownUIDs[3][4];

// Namen per UID
extern const char* eigenaarNamen[3];

// Functie die UID controleert en index teruggeeft
int checkUID(byte *uid);

#endif
