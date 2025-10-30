#include "MCP4151.h"

MCP4151 Mcp4151(2, 3, 4);

void setup() {
  // put your setup code here, to run once:
  Mcp4151.begin();
  Mcp4151.setWiper(0);
}

void loop() {
  // put your main code here, to run repeatedly:

  /*
  if (Serial.available()) {
    uint16_t value = Serial.parseInt();
    Serial.print("Setting wiper to: ");
    Serial.print(value);
    Serial.print(" = ");
    printBits(value);
    Mcp4151.setWiper(value);
  } 
  */
  for (int i=0; i <= 256; i++) {
    Mcp4151.setWiper(i);
    delay(200);
  }
}






