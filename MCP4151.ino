
void setup() {
  // put your setup code here, to run once:
  MCP4151_init();
  MCP4151_set_wiper(0);
  Serial.begin(9600);
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
    MCP4151_set_wiper(value);
  } 
  */
  for (int i=0; i <= 256; i++) {
    MCP4151_set_wiper(i);
    //delay(50);
  }
}

void printBits(uint16_t val) {
  Serial.print("0b");
  for (int i = 15; i >= 0; i--) {           // von MSB nach LSB
    Serial.print((val >> i) & 1);           // Bit extrahieren und ausgeben
    //if (i % 4 == 0) Serial.print(' ');      // optional: kleine Gruppen für Lesbarkeit
  }
  Serial.println();
}




