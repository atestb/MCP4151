#include <MCP4151.h>

MCP4151 Mcp4151(2, 3, 4);

void setup() {
  Mcp4151.begin();
  // set the potentiometer value as desired (here: mid-point of 0..256)
  Mcp4151.setWiper(128);
}

void loop() {}






