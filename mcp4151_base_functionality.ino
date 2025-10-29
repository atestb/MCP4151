#define PIN_CONNECTED_TO_MCP4151_CS_PIN 2
#define PIN_CONNECTED_TO_MCP4151_SCK_PIN 3
#define PIN_CONNECTED_TO_MCP4151_SDIO_PIN 4 // HINT: real 3-wire spi also on arduino side!

void MCP4151_init() {
  pinMode(PIN_CONNECTED_TO_MCP4151_CS_PIN, OUTPUT);
  pinMode(PIN_CONNECTED_TO_MCP4151_SCK_PIN, OUTPUT);
  pinMode(PIN_CONNECTED_TO_MCP4151_SDIO_PIN, OUTPUT);
  
  chipSelectMCP4151(false);
  clockSet(LOW);
  dataLineSet(false);
}

void chipSelectMCP4151(bool selected) {
  uint8_t chipSelect = selected ? LOW : HIGH;
  digitalWrite(PIN_CONNECTED_TO_MCP4151_CS_PIN, chipSelect);
}

void clockSet(uint8_t level) {
  digitalWrite(PIN_CONNECTED_TO_MCP4151_SCK_PIN, level);
}

void dataLineSet(bool bit) {
  uint8_t level = bit ? HIGH : LOW;
  digitalWrite(PIN_CONNECTED_TO_MCP4151_SDIO_PIN, level);
}

void writeSPIBit(bool bit, bool set_pin_to_input_afterwards = false) {
  dataLineSet(bit);
  delayMicroseconds(1);
  clockSet(HIGH);
  delayMicroseconds(1);
  if(set_pin_to_input_afterwards) {
    pinMode(PIN_CONNECTED_TO_MCP4151_SDIO_PIN, INPUT);
  }
  clockSet(LOW);
}

bool readSPIBit(bool set_pin_to_output_afterwards = false) {
  delayMicroseconds(1);
  clockSet(HIGH);
  bool read_bit = (bool)(digitalRead(PIN_CONNECTED_TO_MCP4151_SDIO_PIN));
  delayMicroseconds(1);
  if(set_pin_to_output_afterwards) {
    pinMode(PIN_CONNECTED_TO_MCP4151_SDIO_PIN, OUTPUT);
  }
  clockSet(LOW);
  return read_bit;
}

void MCP4151_send_write_command(uint16_t command) {
  // write the first 4 bits to the line: these represent the target ADDRESS BITS of the command
  // bits and do have the bit offsets from 15 down to 12 (15:12)
  for (int i = 15; i >= 12; i--) {
    writeSPIBit(get_bit_at_position(command, i));
  }

  // the next two bits, 11:10 give the 2-bit opcode the ic must execute (like write operation)
  // but bit No. 5 is written regular, but bit 6 mus be written such that before the falling flank of bit 6
  // the MOSI/COPI port is in high z for receiving the error code of the MCP4151 on the next rising flank
  writeSPIBit(get_bit_at_position(command,11));
  // then write the 6th bit but immediately go to high-z state for receiving 7th bit error code after
  writeSPIBit(get_bit_at_position(command, 10), true);

  // read the 7th bit in high-z state and immediately go to OUTPUTState for writing 7th bit
  bool error_code_bit = readSPIBit(true);

  // write the 8th bit to finish writing the first command byte
  writeSPIBit(get_bit_at_position(command, 8));

  // write the last 8 bits of the command (the last byte)
  for(int i = 7; i >= 0; i--) {
    writeSPIBit(get_bit_at_position(command, i));
  }
}

bool get_bit_at_position(uint16_t value, uint8_t position) {
  return ((value >> position) & 1) > 0 ? true : false;
}