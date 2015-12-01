/***************************************************
  This is our touchscreen painting example for the Adafruit ILI9341
  captouch shield
  ----> http://www.adafruit.com/products/1947

  Check out the links above for our tutorials and wiring diagrams

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

void setup(void) {
  while (!Serial);     // used for leonardo debugging

  Serial.begin(115200);
  Serial.println(F("Cap Touch Paint!"));

  tft.begin();

  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  Serial.println("Capacitive touchscreen started");

  tft.fillScreen(ILI9341_BLACK);

  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(0, BOXSIZE * 2, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(0, BOXSIZE * 3, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(0, BOXSIZE * 4, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.fillRect(0, BOXSIZE * 5, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
  tft.fillRect(0, BOXSIZE * 6, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  tft.fillRect(0, BOXSIZE * 7, BOXSIZE, BOXSIZE, ILI9341_ORANGE);


  //draw the grid
  tft.drawLine(0, 0, 0, 280, ILI9341_WHITE);
  tft.drawLine(40, 0, 40, 280, ILI9341_WHITE);
  tft.drawLine(80, 0, 80, 280, ILI9341_WHITE);
  tft.drawLine(120, 0, 120, 280, ILI9341_WHITE);
  tft.drawLine(160, 0, 160, 280, ILI9341_WHITE);
  tft.drawLine(200, 0, 200, 280, ILI9341_WHITE);
  tft.drawLine(239, 0, 239, 280, ILI9341_WHITE);


  tft.drawLine(0, 0, 240, 0, ILI9341_WHITE);
  tft.drawLine(0, 40, 240, 40, ILI9341_WHITE);
  tft.drawLine(0, 80, 240, 80, ILI9341_WHITE);
  tft.drawLine(0, 120, 240, 120, ILI9341_WHITE);
  tft.drawLine(0, 160, 240, 160, ILI9341_WHITE);
  tft.drawLine(0, 200, 240, 200, ILI9341_WHITE);
  tft.drawLine(0, 240, 240, 240, ILI9341_WHITE);
  tft.drawLine(0, 280, 240, 280, ILI9341_WHITE);





  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
}

void loop() {
  // Wait for a touch
  if (! ctp.touched()) {
    return;
  }

  // Retrieve a point
  TS_Point p = ctp.getPoint();

  /*
   // Print out raw data from screen touch controller
   Serial.print("X = "); Serial.print(p.x);
   Serial.print("\tY = "); Serial.print(p.y);
   Serial.print(" -> ");
  */

  // flip it around to match the screen.
  p.x = map(p.x, 0, 240, 240, 0);
  p.y = map(p.y, 0, 320, 320, 0);

  // Print out the remapped (rotated) coordinates
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");


  if (p.x < BOXSIZE) {
    oldcolor = currentcolor;

    if (p.y < BOXSIZE) {
      currentcolor = ILI9341_RED;
      tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y < BOXSIZE * 2) {
      currentcolor = ILI9341_YELLOW;
      tft.drawRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y < BOXSIZE * 3) {
      currentcolor = ILI9341_GREEN;
      tft.drawRect(0, BOXSIZE * 2, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y < BOXSIZE * 4) {
      currentcolor = ILI9341_CYAN;
      tft.drawRect(0, BOXSIZE * 3, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y < BOXSIZE * 5) {
      currentcolor = ILI9341_BLUE;
      tft.drawRect(0, BOXSIZE * 4, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y <= BOXSIZE * 6) {
      currentcolor = ILI9341_MAGENTA;
      tft.drawRect(0, BOXSIZE * 5, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    } else if (p.y <= BOXSIZE * 7) {
      currentcolor = ILI9341_WHITE;
      tft.drawRect(0, BOXSIZE * 6, BOXSIZE, BOXSIZE, ILI9341_RED);
    } else if (p.y <= BOXSIZE * 8) {

      tft.fillScreen(ILI9341_BLACK);
      tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
      tft.fillRect(0, BOXSIZE , BOXSIZE, BOXSIZE, ILI9341_YELLOW);
      tft.fillRect(0, BOXSIZE * 2, BOXSIZE, BOXSIZE, ILI9341_GREEN);
      tft.fillRect(0, BOXSIZE * 3, BOXSIZE, BOXSIZE, ILI9341_CYAN);
      tft.fillRect(0, BOXSIZE * 4, BOXSIZE, BOXSIZE, ILI9341_BLUE);
      tft.fillRect(0, BOXSIZE * 5, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
      tft.fillRect(0, BOXSIZE * 6, BOXSIZE, BOXSIZE, ILI9341_WHITE);
      tft.fillRect(0, BOXSIZE * 7, BOXSIZE, BOXSIZE, ILI9341_ORANGE);


      //draw the grid
      tft.drawLine(0, 0, 0, 280, ILI9341_WHITE);
      tft.drawLine(40, 0, 40, 280, ILI9341_WHITE);
      tft.drawLine(80, 0, 80, 280, ILI9341_WHITE);
      tft.drawLine(120, 0, 120, 280, ILI9341_WHITE);
      tft.drawLine(160, 0, 160, 280, ILI9341_WHITE);
      tft.drawLine(200, 0, 200, 280, ILI9341_WHITE);
      tft.drawLine(239, 0, 239, 280, ILI9341_WHITE);


      tft.drawLine(0, 0, 240, 0, ILI9341_WHITE);
      tft.drawLine(0, 40, 240, 40, ILI9341_WHITE);
      tft.drawLine(0, 80, 240, 80, ILI9341_WHITE);
      tft.drawLine(0, 120, 240, 120, ILI9341_WHITE);
      tft.drawLine(0, 160, 240, 160, ILI9341_WHITE);
      tft.drawLine(0, 200, 240, 200, ILI9341_WHITE);
      tft.drawLine(0, 240, 240, 240, ILI9341_WHITE);
      tft.drawLine(0, 280, 240, 280, ILI9341_WHITE);


    }


    if (oldcolor != currentcolor) {
      if (oldcolor == ILI9341_RED)
        tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
      if (oldcolor == ILI9341_YELLOW)
        tft.fillRect(0, BOXSIZE, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
      if (oldcolor == ILI9341_GREEN)
        tft.fillRect(0, BOXSIZE * 2, BOXSIZE, BOXSIZE, ILI9341_GREEN);
      if (oldcolor == ILI9341_CYAN)
        tft.fillRect(0, BOXSIZE * 3, BOXSIZE, BOXSIZE, ILI9341_CYAN);
      if (oldcolor == ILI9341_BLUE)
        tft.fillRect(0, BOXSIZE * 4, BOXSIZE, BOXSIZE, ILI9341_BLUE);
      if (oldcolor == ILI9341_MAGENTA)
        tft.fillRect(0, BOXSIZE * 5, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
      if (oldcolor == ILI9341_WHITE)
        tft.fillRect(0, BOXSIZE * 6, BOXSIZE, BOXSIZE, ILI9341_WHITE);
      if (oldcolor == ILI9341_ORANGE)
        tft.fillRect(0, BOXSIZE * 7, BOXSIZE, BOXSIZE, ILI9341_ORANGE);

    }
  }
  if (((p.y - PENRADIUS) > BOXSIZE) && ((p.y + PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }


}
