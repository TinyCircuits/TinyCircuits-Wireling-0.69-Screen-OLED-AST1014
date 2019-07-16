//*******************************************************************
// TinyCircuits - SSD1306 OLED Display Library
// Written by: Brandon Farmer for TinyCircuits
//
// Initiated: Fri. 11/24/2017 @ 12:30PM
//   Updated: Fri. 11/24/2017 @ 12:45PM
//
// Revision 0 - Original Release
//	RELEASED: MM/DD/YYYY ***Not Yet Released***
//
//*******************************************************************

#include <stdlib.h>
#include <Arduino.h>
#include <Wire.h>
#include "TinyCircuits_SSD1306.h"

//Constructor
TinyCircuits_SSD1306::TinyCircuits_SSD1306(void) {
  //Nothing
}

//Start the I2C Communication
void TinyCircuits_SSD1306::begin(void) {
  Wire.begin();
}

//Send Command Function 
void TinyCircuits_SSD1306::sendCommand(uint8_t command) {
  Wire.beginTransmission(SSD1306_DEFAULT_ADDRESS);
  Wire.write(0x00);
  Wire.write(command);
  Wire.endTransmission();
}

//Screen Initialization
void TinyCircuits_SSD1306::init(void) {
  // Turn display off
  sendCommand(SSD1306_DISPLAYOFF);

  sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
  sendCommand(0x80);

  sendCommand(SSD1306_SETMULTIPLEX);
  sendCommand(0x0F);

  sendCommand(SSD1306_SETDISPLAYOFFSET);
  sendCommand(0x00);

  sendCommand(SSD1306_SETSTARTLINE | 0x00);

  // We use internal charge pump
  sendCommand(SSD1306_CHARGEPUMP);
  sendCommand(0x14);

  sendCommand(SSD1306_SEGREMAP | 0x1);

  sendCommand(SSD1306_COMSCANDEC);

  sendCommand(SSD1306_SETCOMPINS);
  sendCommand(0x02);

  // Max contrast
  sendCommand(SSD1306_SETCONTRAST);
  sendCommand(0xAF);

  sendCommand(SSD1306_SETPRECHARGE);
  sendCommand(0xF1);

  sendCommand(SSD1306_SETVCOMDETECT);
  sendCommand(0x20);

  sendCommand(SSD1306_DISPLAYALLON_RESUME);

  // Non-inverted display
  sendCommand(SSD1306_NORMALDISPLAY);

  // Turn display back on
  sendCommand(SSD1306_DISPLAYON);


  // Horizontal memory mode
  sendCommand(SSD1306_MEMORYMODE);
  sendCommand(0x00);
}

void TinyCircuits_SSD1306::sendFramebuffer(uint8_t *buffer) {
    sendCommand(SSD1306_COLUMNADDR);
  sendCommand(0x00);
  sendCommand(0x5F);

  sendCommand(SSD1306_PAGEADDR);
  sendCommand(0x00);
  sendCommand(0x01);

  // We have to send the buffer as 16 bytes packets
  // Our buffer is 1024 bytes long, 1024/16 = 64
  // We have to send 64 packets
  for (uint8_t packet = 0; packet < 24; packet++) {
    Wire.beginTransmission(SSD1306_DEFAULT_ADDRESS);
    Wire.write(0x40);
    for (uint8_t packet_byte = 0; packet_byte < 8; ++packet_byte) {
      Wire.write(buffer[packet * 8 + packet_byte]);
    }
    Wire.endTransmission();
  }
}












