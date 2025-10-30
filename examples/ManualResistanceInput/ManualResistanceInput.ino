#include <MCP4151.h>

MCP4151 Mcp4151(2, 3, 4);

void setup() {
  // put your setup code here, to run once:
  Mcp4151.begin();
  Mcp4151.setWiper(0);
  Serial.begin(115200);
  Mcp4151.startDebugOutputOverSerial();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    uint16_t value = Serial.parseInt();
    String input_repeat = "Setting wiper to: " + String(value);
    Serial.println(input_repeat);
    Mcp4151.setWiper(value);
  }
}






