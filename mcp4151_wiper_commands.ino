// 9bits can be set: bit 0 to 7 set the wiper higher. 
// bit 8 is special: if set, bits 0 to 8 do not matter: wiper has max value anyways
// NOTE: bits 9 to 15 do not get taken into account for setting the wiper
void MCP4151_set_wiper  (uint16_t value) {
  // first 4 bits: address (0b0000 is address 0)
  // next 2 bits: opcode (0b00 is write)
  // next 1 bit: doesnt matter / error condition bit (!)
  // next 9 bits: data to write into address, not set here - see further down in code
  uint16_t writeCommandWithoutData = 0x0000;
  // now blend in the data in bits 0:8 from the function argument
  uint16_t mask = 0x01FF;
  uint16_t writeCommand = (writeCommandWithoutData & ~mask) | (value & mask);
  Serial.begin(9600);
  Serial.print("Sending command: ");
  printBits(writeCommand);
  Serial.println();

  chipSelectMCP4151(true);
  MCP4151_send_write_command(writeCommand);
  chipSelectMCP4151(false);
}
