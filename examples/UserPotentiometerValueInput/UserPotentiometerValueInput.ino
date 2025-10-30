#include <MCP4151.h>

MCP4151 Mcp4151(2, 3, 4);

void setup() {
  Mcp4151.begin();
  Mcp4151.setWiper(0);
  Serial.begin(115200);
  // next line is optional (if you want debug output)
  Mcp4151.startDebugOutputOverSerial();
}

void loop() {
  if (Serial.available()) {
    uint16_t value = Serial.parseInt();
    String input_repeat = "Setting potentiometer to: " + String(value);
    Serial.println(input_repeat);
    Mcp4151.setWiper(value);
  }
}






