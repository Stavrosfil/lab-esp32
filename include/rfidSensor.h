#ifndef RFID_H
#define RFID_H
#include <MFRC522.h>
#include <SPI.h>
#include <definitions.h>

void initializeRFIDSensor(MFRC522 &sensor);

void enableRFIDInterrupts(MFRC522 &sensor);

void activateRFIDReception(MFRC522 &mfrc522);

String readCardIRQ(MFRC522 &sensor);

String readCard(MFRC522 &sensor);

#endif
