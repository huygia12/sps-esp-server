#include "SPS_RFID_Scanner.h"

SPS_RFID_Scanner::SPS_RFID_Scanner(int ssPin, int rstPin)
    : rfid(ssPin, rstPin), isLastCardValid(false) {}

void SPS_RFID_Scanner::init(byte **validUIDs, int totalValidUIDs) {
  SPI.begin();
  rfid.PCD_Init();
  delay(4);
  this->validUIDs = validUIDs;
  this->totalValidUIDs = totalValidUIDs;
}
int SPS_RFID_Scanner::validateCard() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return -1;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    isLastCardValid = false;
    return -1;
  }

  for (int i = 0; i < totalValidUIDs; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) {
      if (rfid.uid.uidByte[j] != validUIDs[i][j]) {
        match = false;
        break;
      }
    }

    if (match) {
      isLastCardValid = true;
      return i;
    }
  }

  isLastCardValid = false;

  return -1;
}

void SPS_RFID_Scanner::clearCache() { isLastCardValid = false; };