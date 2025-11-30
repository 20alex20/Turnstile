#include "RFIDReader.h"

RFIDReader::RFIDReader(uint8_t ssPin, uint8_t rstPin) {
    reader = new MFRC522(ssPin, rstPin);
    reader->PCD_Init();
}

bool RFIDReader::readCardID(byte* uid, byte& uidSize) {
    if (!reader->PICC_ReadCardSerial()) {
        return false;
    }

    uidSize = reader->uid.size;
    if (uidSize <= 10) {
        for (byte i = 0; i < uidSize; i++) {
            uid[i] = reader->uid.uidByte[i];
        }
        stopReading();
        return true;
    }
    stopReading();
    return false;
}

void RFIDReader::stopReading() {
    reader->PICC_HaltA();
    reader->PCD_StopCrypto1();
}
