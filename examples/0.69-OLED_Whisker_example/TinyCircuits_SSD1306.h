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

#ifndef _TinyCircuits_SSD1306_H_
#define _TinyCircuits_SSD1306_H_

#include <Arduino.h>
#include <Wire.h>

#define SSD1306_DEFAULT_ADDRESS     (0x3C)
#define SSD1306_SETCONTRAST         (0x81)
#define SSD1306_DISPLAYALLON_RESUME (0xA4)
#define SSD1306_DISPLAYALLON        (0xA5)
#define SSD1306_NORMALDISPLAY       (0xA6)
#define SSD1306_INVERTDISPLAY       (0xA7)
#define SSD1306_DISPLAYOFF          (0xAE)
#define SSD1306_DISPLAYON           (0xAF)
#define SSD1306_SETDISPLAYOFFSET    (0xD3)
#define SSD1306_SETCOMPINS          (0xDA)
#define SSD1306_SETVCOMDETECT       (0xDB)
#define SSD1306_SETDISPLAYCLOCKDIV  (0xD5)
#define SSD1306_SETPRECHARGE        (0xD9)
#define SSD1306_SETMULTIPLEX        (0xA8)
#define SSD1306_SETLOWCOLUMN        (0x00)
#define SSD1306_SETHIGHCOLUMN       (0x10)
#define SSD1306_SETSTARTLINE        (0x40)
#define SSD1306_MEMORYMODE          (0x20)
#define SSD1306_COLUMNADDR          (0x21)
#define SSD1306_PAGEADDR            (0x22)
#define SSD1306_COMSCANINC          (0xC0)
#define SSD1306_COMSCANDEC          (0xC8)
#define SSD1306_SEGREMAP            (0xA0)
#define SSD1306_CHARGEPUMP          (0x8D)
#define SSD1306_SWITCHCAPVCC        (0x2)
#define SSD1306_NOP                 (0xE3)

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8

class TinyCircuits_SSD1306 {

  public:
    TinyCircuits_SSD1306(void); 		//Constructor

    void begin(void);
    void sendCommand(uint8_t); 
    void init(void); 
    void sendFramebuffer(uint8_t *buffer);

  private:
    //none
    
};

#endif		//TinyCircuits_SSD1306_H_ 
