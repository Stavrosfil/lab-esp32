#include <rfidSensor.h>

void initializeRFIDSensor(MFRC522 &sensor) {

    // Init SPI bus
    SPI.begin();

    // Init MFRC522
    sensor.PCD_Init();

    // Set sensor antenna gain to maximum.
    sensor.PCD_SetAntennaGain(sensor.RxGain_48dB);

// Show details of PCD - MFRC522 Card Reader details
#ifdef DEBUG
    sensor.PCD_DumpVersionToSerial();
#endif

    DEBUG_PRINTLN(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void enableRFIDInterrupts(MFRC522 &sensor) {

    pinMode(IRQ_PIN, INPUT_PULLUP);

    // Allow the ... irq to be propagated to the IRQ pin
    // For test purposes propagate the IdleIrq and loAlert
    // regVal = 0xA0; // rx irq
    sensor.PCD_WriteRegister(sensor.ComIEnReg, 0xA0);
}

//  The function sending to the MFRC522 the needed commands to activate the
//  reception
void activateRFIDReception(MFRC522 &mfrc522) {

    mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_REQA);
    // mfrc522.PCD_WriteRegister(mfrc522.FIFODataReg, mfrc522.PICC_CMD_WUPA);

    mfrc522.PCD_WriteRegister(mfrc522.CommandReg, mfrc522.PCD_Transceive);

    mfrc522.PCD_WriteRegister(mfrc522.BitFramingReg, 0x87);
}

String readCardIRQ(MFRC522 &sensor) {

    String content = "";

    DEBUG_PRINT(F("Interrupt. "));
    sensor.PICC_ReadCardSerial(); // read the tag data
    // Show some details of the PICC (that is: the tag/card)
    DEBUG_PRINT(F("Card UID:"));
    // dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

    byte *buffer   = sensor.uid.uidByte;
    int bufferSize = sensor.uid.size;

    for (byte i = 0; i < bufferSize; i++) {
        DEBUG_PRINT(buffer[ i ] < 0x10 ? " 0" : " ");
        DEBUG_PRINTHEX(buffer[ i ]);

        // Append it to the variable
        content += String(buffer[ i ] < 0x10 ? "0" : "");
        content += String(buffer[ i ], HEX);
    }

    DEBUG_PRINTLN();

    // sensor.PCD_WriteRegister(sensor.ComIrqReg, 0x80); // Clear
    sensor.PCD_WriteRegister(sensor.ComIrqReg, 0x7F);
    // sensor.PCD_WriteRegister(sensor.FIFOLevelReg, 0x80);
    // sensor.PCD_ClearRegisterBitMask(sensor.FIFODataReg, 0x00);

    sensor.PICC_HaltA();

    content.toUpperCase();

    return content;
}

String readCard(MFRC522 &sensor) {

    // Look for new cards
    if (!sensor.PICC_IsNewCardPresent()) {
        return "";
    }
    // Select one of the cards
    if (!sensor.PICC_ReadCardSerial()) {
        return "";
    }
    // Show UID on serial monitor
    DEBUG_PRINT("UID tag :");
    String content = "";
    for (byte i = 0; i < sensor.uid.size; i++) {

        // Print it to the serial
        DEBUG_PRINT(sensor.uid.uidByte[ i ] < 0x10 ? " 0" : " ");
        DEBUG_PRINTHEX(sensor.uid.uidByte[ i ]);

        // Append it to the variable
        content.concat(String(sensor.uid.uidByte[ i ] < 0x10 ? "0" : ""));
        content.concat(String(sensor.uid.uidByte[ i ], HEX));
    }

    content.toUpperCase();

    return content;
}
