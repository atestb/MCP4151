#include "MCP4151.h"
#include "Utils.h"

MCP4151::MCP4151(uint8_t chipselectPin, uint8_t clockPin, uint8_t dataPin)
  : _chipselectPin(chipselectPin), _clockPin(clockPin), _dataPin(dataPin), _debugOutputOverSerial(false) {}

void MCP4151::begin(void) {
  pinMode(_chipselectPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  chipSelect(false);
  clockSet(LOW);
  dataLineSet(false);
}

// TODO: document the baud rate specifics of this for the end-user of the lib
// not complicated, but people starting the default 9600 serial instead of the explicit 15200
// terminal intended here might get rubbish
void MCP4151::startDebugOutputOverSerial(void) {
  if (Serial) {
    _debugOutputOverSerial = true;
  }
}

void MCP4151::stopDebugOutputOverSerial(void) {
  // don't stop the serial, since we cannot be sure if we started it.
  // but stop the methods from outputting debug information by:
  _debugOutputOverSerial = false;
}

void MCP4151::chipSelect(bool select) {
  uint8_t chipSelectLevel = select ? LOW : HIGH; // CS pin is active low in SPI protocol
  digitalWrite(_chipselectPin, chipSelectLevel);
}

void MCP4151::clockSet(uint8_t level) {
  digitalWrite(_clockPin, level);
}

void MCP4151::dataLineSet(bool bit) {
  uint8_t level = bit ? HIGH : LOW;
  digitalWrite(_dataPin, level);
}

// 9bits can be set: bit 0 to 7 set the wiper higher. 
// bit 8 is special: if set, bits 0 to 8 do not matter: wiper has max value anyways
// NOTE: bits 9 to 15 do not get taken into account for setting the wiper
void MCP4151::setWiper(uint16_t value) {
  // first 4 bits: address (0b0000 is address 0)
  // next 2 bits: opcode (0b00 is write)
  // next 1 bit: doesnt matter / error condition bit (!)
  // next 9 bits: data to write into address, not set here - see further down in code
  uint16_t writeCommandWithoutData = 0x0000;
  // now blend in the data in bits 0:8 from the function argument
  uint16_t mask = 0x01FF;
  uint16_t writeCommand = (writeCommandWithoutData & ~mask) | (value & mask);
  
  if(_debugOutputOverSerial) {
    Serial.print("Sending command: ");
    printBits(writeCommand);
    Serial.println();
  }

  chipSelect(true);
  sendWriteCommand(writeCommand);
  chipSelect(false);
}

void MCP4151::writeSPIBit(bool bit, bool set_pin_to_input_afterwards) {
  dataLineSet(bit);
  delayMicroseconds(1);
  clockSet(HIGH);
  delayMicroseconds(1);
  /* now comes the very special case that the next bit is known to be a read bit
  * instead of a write bit.
  * This is the case for e.g. bit 7 in the first command byte transferred to the MCP4151,
  * where there is an error bit feedback from MCP4151 to the Master/Controller. 
  * 
  * (To get more info about bit 7 or when else the data line changes to read (hint: read command) during
  * SPI command transfer, please read the MCP4151 datasheet sections about SPI interfacing with the chip.)
  * 
  * So in these cases, when the next bit is known to be a read bit instead of a written bit:
  * before clock is set to low, we switch the data pin to input
  * reason for this timing before the LOW flank of the clock: 
  * MCP4151 drives the line with voltage level on falling flank of the clock (datasheet says that)
  * and then if we let that happen with data line still as output,
  * we could have for some time two outputs on the same line coming from 2 sides 
  * (MCP4151 and arduino pins driving at the same time)
  * to be more precise: if the one side then drives the line low and the other high, short circuit current would flow
  * we dont want short circuit current. (btw: that is the reason for the 330ohm resistor described in TODO: readme.md)
  */
  if(set_pin_to_input_afterwards) {
    pinMode(_dataPin, INPUT);
  }
  clockSet(LOW);
}

bool MCP4151::readSPIBit(bool set_pin_to_output_afterwards) {
  delayMicroseconds(1);
  clockSet(HIGH);
  bool read_bit = (bool)(digitalRead(_dataPin));
  delayMicroseconds(1);
  // see comment in writeSPIBit method what is happening here before clock LOW flank optionally and why
  if(set_pin_to_output_afterwards) {
    pinMode(_dataPin, OUTPUT);
  }
  clockSet(LOW);
  return read_bit;
}

bool MCP4151::get_bit_at_position(uint16_t value, uint8_t position) {
  return ((value >> position) & 1) > 0 ? true : false;
}

void MCP4151::sendWriteCommand(uint16_t command) {
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

