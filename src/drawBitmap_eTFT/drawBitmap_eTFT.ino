// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/04/18

#include "xbm.h"             // Sketch tab header for xbm images

#include <TFT_eSPI.h>        // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();   // Invoke library


void setup()
{

  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
  
  tft.begin();               // Initialise the display

  
  tft.fillScreen(TFT_WHITE); // Black screen fill

  tft.setSwapBytes(true);
  tft.pushImage(0, ((320/2)-logoHeight/2), logoWidth, logoHeight, logo);
  
}

void loop()
{

  

  /*
  
  // Example 1
  // =========
  // Random x and y coordinates
  int x = random(tft.width()  - logoWidth);
  int y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //              x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE);

  delay(500);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK);


  // Example 2
  // =========
  // New random x and y coordinates
  x = random(tft.width()  - logoWidth);
  y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE, TFT_RED);

  delay(500);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK, TFT_BLACK);
*/
}

/***************************************************************************************
** Function name:           drawBitmap
** Description:             Draw an image stored in an array on the TFT
***************************************************************************************/
//void drawBitmapPhil(int16_t x, int16_t y, const unsigned short *bitmap, int16_t w, int16_t h, uint16_t color)
//{
//  begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
//  //inTransaction = true;
//
//  int32_t i, j, byteWidth = (w + 7) / 8;
//
//  for (j = 0; j < h; j++) {
//    for (i = 0; i < w; i++ ) {
//      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
//        drawPixel(x + i, y + j, color);
//      }
//    }
//  }
//
//  //inTransaction = false;
//  end_tft_write();              // Does nothing if Sprite class uses this function
//}
