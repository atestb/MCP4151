#pragma once

#include <Arduino.h>

class MCP4151 {

public:
  MCP4151(uint8_t chipselectPin, uint8_t clockPin, uint8_t dataPin);
  void begin(void);
  void setWiper(uint16_t value);

private:
  uint8_t _chipselectPin;
  uint8_t _clockPin;
  uint8_t _dataPin;
  void chipSelect(bool select); // true if you want to select, false else: logical, dont give voltage level here
  void clockSet(uint8_t level); // set clock HIGH or LOW (use these constants)
  void dataLineSet(bool bit); // write a 1 (true) or 0 (false) on the data line / data pin
  void writeSPIBit(bool bit, bool set_pin_to_input_afterwards = false);
  bool readSPIBit(bool set_pin_to_output_afterwards = false);
  static bool get_bit_at_position(uint16_t value, uint8_t position);
  void sendWriteCommand(uint16_t command);
};