#include <MCP4151.h>

MCP4151 Mcp4151(2, 3, 4);

void setup() {
  Mcp4151.begin();
  // initialize the potentiometer value to something reasonable (0) first
  Mcp4151.setWiper(0);
}

void loop() {
  // change the potentiometer value every 100 milliseconds:
  for (int i=0; i <= 256; i++) {
    Mcp4151.setWiper(i);
    delay(100);
  }
}






