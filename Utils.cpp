#include "Utils.h"

void printBits(uint16_t val) {
  Serial.print("0b");
  for (int i = 15; i >= 0; i--) {           // from MSB to LSB
    Serial.print((val >> i) & 1);           // extract 1 bit and print it
  }
  Serial.println();
}