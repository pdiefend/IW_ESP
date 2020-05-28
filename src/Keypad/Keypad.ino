
#include "FS.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

// pin definitions
#define TFT_LED  D8
#define TOUCH_INT D4


//#define CAL_TS

#define TFT_X_SIZE  240
#define TFT_Y_SIZE  320

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// not working presently
//void ICACHE_RAM_ATTR TouchISR ();

void setup() {
  // Use serial port
  Serial.begin(115200);

  // Initialise the TFT screen
  tft.init();

  // turn on the backlight, this is PWM capable but do full brightness for now
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Set the rotation before we calibrate
  tft.setRotation(0);

  // Calibrate the touch screen and retrieve the scaling factors
  #ifdef CAL_TS
  touch_calibrate();
  #endif
  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  // Draw keypad background
  tft.fillRect(0, 0, TFT_X_SIZE, TFT_Y_SIZE, TFT_DARKGREY);

  // Draw keypad
  drawKeypad();

  // Setup Touch IRQ (Not working presently)
  //attachInterrupt(digitalPinToInterrupt(TOUCH_INT), TouchISR, FALLING);
  
}

int pinState = 0, lastPinState = 0;
uint16_t t_x = 0, t_y = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //Serial.println(pinState);

  
  // get touch is rotated from what the library expects
  if(tft.getTouch(&t_y, &t_x)){
    int xIdx, yIdx;
    
    if(t_x >= 40){
      xIdx = (t_x-40) / 80;  
    } else {
      xIdx = 0;
    }
    
    if(t_y >= 64){
      yIdx = (t_y - 64) / 96;
    } else {
      yIdx = -1;
    }

    Serial.print(xIdx);
    Serial.print(" ");
    Serial.print(yIdx);
    Serial.print("; ");
    Serial.print(t_x);
    Serial.print(" ");
    Serial.print(t_y);
    Serial.println();
    
    /*
    if(x > 20 && x <60){
      // 1 pressed
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
      
    } else if ((x > 20 && x <60){
    */
  } else {
    delay(30);
  }
  
  
  /*
  while (lastPinState == pinState){
    pinState = digitalRead(D4); 
    delay(1);
  }
  lastPinState = pinState;
  Serial.println(pinState);
  */
  
}

void drawKeypad() {
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE);
  //tft.setFreeFont(&FreeSans18pt7b);                 // Select the font
  //tft.drawString("Sans 24", 160, 60, GFXFF);// Print the string name of the font
  tft.setFreeFont(&FreeSans18pt7b);
  //tft.drawString("1234567890", 160, 120, GFXFF);
  tft.drawString("1", 40, 96);
  tft.drawString("2", 120, 96);
  tft.drawString("3", 200, 96);
  tft.drawString("4", 40, 160);
  tft.drawString("5", 120, 160);
  tft.drawString("6", 200, 160);
  tft.drawString("7", 40, 224);
  tft.drawString("8", 120, 224);
  tft.drawString("9", 200, 224);
  tft.drawString("C", 40, 288); // get better icon for clear
  tft.drawString("0", 120, 288);
  tft.drawString("->", 200, 288); // get better icon for enter

  tft.drawLine(0, 64, 240, 64, TFT_BLACK);
}

/*
// not working presently, don't forget IRAM declaration
void TouchISR(){
  digitalWrite(LED_BUILTIN, HIGH);
}
*/


//==================================================================

#ifdef CAL_TS
// TODO move this to from SD card to EEPROM
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
#endif
