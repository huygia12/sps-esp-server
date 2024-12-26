#ifndef SPS_RFID_Scanner_H
#define SPS_RFID_Scanner_H

#include <MFRC522.h>
#include <SPI.h>

class SPS_RFID_Scanner {
public:
  SPS_RFID_Scanner(int ssPin, int rstPin);
  int validateCard();
  void init(byte **validUIDs, int totalValidUIDs);
  void clearCache();

private:
  MFRC522 rfid;
  byte **validUIDs;
  int totalValidUIDs;
  bool isLastCardValid;
};

#endif