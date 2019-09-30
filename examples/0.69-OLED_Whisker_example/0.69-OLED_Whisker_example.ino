/************************************************************************
 * SSD1306 0.69" OLED Display Whisker Demo/Test Code -
 * This code will display text, and a Sprite from the exampleSprites.h at 
 * the same time when uploaded. 
 *
 * Hardware by: TinyCircuits
 * Written by: Ben Rose, Laverena Wienclaw for TinyCircuits
 *
 * Initiated: Mon. 11/20/2017 
 * Updated: Tue. 07/03/2018
 ************************************************************************/

#include <Wire.h>                   // For I2C communication
#include <TinyScreen.h>             // For interfacing with TinyScreen+
#include "fontorig.h"               // The font displayed on the screen
#include "TinyCircuits_SSD1306.h"   // Library for OLED screen
#include "exampleSprites.h"         // Holds arrays of example Sprites

// Variable for the 0.69" Whisker screen
TinyCircuits_SSD1306 OLEDport1;

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// This is the data structure for a TinyScreen Sprite
typedef struct {
  int x;             // X coordinate (top left corner) of where Sprite is placed on screen initially 
  int y;             // Y coordinate (top left corner) of where Sprite is placed on screen initially 
  int width;         // Wideness of Sprite
  int height;        // Height of Sprite
  int bitmapNum;     // Index of animation stage of Sprite (only used when a Sprite is animated)
  const unsigned int * bitmap; // The array of 0's and 1's that determines appearance of Sprite
} Sprite;

// The Sprite is initialized to appear on the screen at coordinate (41, 2)
// and exist in a space of 13 by 13 pixels. The 13 by 13 Bitmap of pixel information
// is assigned in file exampleSprites.h
Sprite bunnySprite = {41, 2, 13, 13, 0, bunnyBitmap};

// Declares how many Sprites are to appear on the screen. This info is used later
// on when the pixel bitmaps are sent to the OLED screen. 
// *** If you want more than one Sprite to appear, it must be added to this list***
int amtSprites = 1; 
Sprite * spriteList[1] = {
  &bunnySprite,   
};

uint8_t buf[2 * 96]; // Buffer to hold screen data
int bufIndex = 0;    // Buffer index in respect to the array of pixels on the screen
int textPos;         // Used to make text move left or right

void setup() {
  Wire.begin();   // Begin I2C communication
  
  /****Resets the Whisker Screen****/
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(10);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  delay(10);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, HIGH);
  delay(10);
  /********************************/

  //Clear Display Buffer, isn't fully cleared here, frame buffer must be sent
  clearOLED();
  delay(2);

  selectPort(0);  // This port# matches the one labeled on the adapter board

  OLEDport1.begin();      // Begin and initialize OLED to display 
  OLEDport1.init();
  OLEDport1.sendFramebuffer(buf);    // Send Cleared Data

  // Initialize appearance of TinyScreen
  display.begin();
  display.setFlip(true);
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, background);
  display.setCursor(0, 0);
  display.print("0.69 OLED Test");

  textPos = 0;      // Text moves left to right >>>
//  textPos = 95;     // Text moves right to left <<<
}

// Clear the screen, put new text on the screen -> clear to create scrolling effect 
void loop() {
  clearOLED();

  bunnySprite.x += 2; // Changes speed of Sprite, bigger increment = faster
  if (bunnySprite.x > 95) bunnySprite.x = -13;

  drawSprites();

  textPos++;      // Text moves left to right >>>
  if (textPos > 95)textPos = 0;

//  textPos--;      // Text moves right to left <<<
//  if (textPos < 0) textPos = 95;

  bufIndex = textPos;
  printSSD("Bunny");
  OLEDport1.sendFramebuffer(buf);
}

// **This function is necessary for all Whisker boards attached through an Adapter board**
// Selects the correct address of the port being used in the Adapter board
void selectPort(int port) {
  Wire.beginTransmission(0x70);
  Wire.write(0x04 + port);
  Wire.endTransmission(0x70);
}

/* The OLED screen is 96 by 16 pixels. The screen uses x and y coordinates,
   but with the twist that the 0,0 point is at the top left:

   **************************************************************************
   |  (0,0), (1,0), (...)                                             (96,0)|
   |  (0,1),                                                                |
   |  (...)                                                                 |
   |                                                                        |
   |                                                                        |
   |                                                                        |
   |  (0,16)                                                         (96,16)|
   **************************************************************************
                              (Not to scale)
  This function draws the Sprite by iterating through y 'slices' to find the x coord
  at which the Sprite starts as defined by the data structure Sprite.
  Each x pixel in these y slices is then flipped on or off using the bool pixelOn

  Fun fact: The x coord must be checked for being on the screen (within 96) due to
  the way the OLED screen works (writes all the information for the top half of the
  screen before starting the bottom half information). That's why there seems to be
  a redundant check for the x coordinate being within position 96.
*/
void drawSprites() {
  display.goTo(0, 0);
  display.startData();

  clearTinierScreen();

  for (int y = 0; y < 16; y++) { // Iterates through all y coordinates of the screen
    for (int spriteIndex = 0; spriteIndex < amtSprites; spriteIndex++) { // Iterates pixels for each Sprite

      // Sets currentSprite to the Current Sprite being accessed in the array
      Sprite *currentSprite = spriteList[spriteIndex]; 
      
      // Checks if the current Y-coordinate is within the current sprite's area
      if (y >= currentSprite->y && (y < (currentSprite->y + currentSprite->height))) { 
        int endX = currentSprite->x + currentSprite->width;
        if (currentSprite->x < 96 && endX > 0) { //checks if the sprite is within the X-coordinate bounds of the screen
          int xBitmapOffset = 0;
          int xStart = 0;
          if (currentSprite->x < 0) {
            xBitmapOffset -= currentSprite->x;
          }
          if (currentSprite->x > 0) {
            xStart = currentSprite->x;
          }
          int yBitmapOffset = (y - currentSprite->y) * currentSprite->width;

          for (int x = xStart; x < endX && x < 96; x++) { //iterates through the X-bounds of the sprite
            bool pixelOn = currentSprite->bitmap[xBitmapOffset + yBitmapOffset++];
            if (pixelOn) {
              setPixel(x, y); //sets the pixel if a 1 is found in the bitmap
            }
          }
        }
      }
    }
  }
}

// Sets all pixels to 0 (turns them all off/black)
void clearTinierScreen() {
  uint8_t lineBuffer[96 * 2];

  for (int bufIndex_temp = 0; bufIndex_temp < 96 * 2; bufIndex_temp++) {
    lineBuffer[bufIndex_temp] = 0x00;
  }
}

// Writes blank data to OLED to clear data
void clearOLED() {
  for (int i = 0; i < 2 * 96; i++) {
    buf[i] = 0x00;
  }
}

// Writes each character of a statement to the screen
void printSSD(char * s) {
  while (*s) {
    write(*s);
    s++;
  }
}

// Expands top byte of data to fit the full screen, one byte -> two bytes
// Background: This screen prints out everything on the top half of the screen first,
//             so this is where we expand that and fill the bottom.
void write(char c) {
  if (c < ' ' || c > ('}' + 2))return;
  c -= ' ';
  for (int i = 0; i < 5; i++) {
    uint8_t h = Font5x7[c * 5 + i];
    if (h) {
      uint8_t l = ((h & (1 << 3)) ? (3 << 6) : 0) | ((h & (1 << 2)) ? (3 << 4) : 0) | ((h & (1 << 1)) ? (3 << 2) : 0) | ((h & (1 << 0)) ? (3 << 0) : 0);
      h = ((h & (1 << 7)) ? (3 << 6) : 0) | ((h & (1 << 6)) ? (3 << 4) : 0) | ((h & (1 << 5)) ? (3 << 2) : 0) | ((h & (1 << 4)) ? (3 << 0) : 0);
      buf[bufIndex] = l;
      buf[96 + bufIndex++] = h;
      if (bufIndex > 95)bufIndex = 0;
      buf[bufIndex] = l;
      buf[96 + bufIndex++] = h;
      if (bufIndex > 95)bufIndex= 0;
    }
  }
  buf[bufIndex] = 0;
  buf[96 + bufIndex++] = 0;
  if (bufIndex > 95)bufIndex = 0;
}

void setPixel(uint8_t px, uint8_t py) {
  uint8_t pos = px;       //holds the given X-coordinate
  if (py > 7)pos += 96;   //if the Y-coordinate is greater than 7, the X position is increased by 96 (the screen width)
  py = py & 7;            //adjusts the Y-coordinate such that it can be written within the 0-7 bounds of a byte
  buf[pos] |= (1 << (py));//the bits of the byte within the buffer are set accordingly by placing a 1 in the respective bit location of the byte
}
