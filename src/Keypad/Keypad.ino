
#include "FS.h"

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
  //drawKeypad();

  // Setup Touch IRQ (Not working presently)
  //attachInterrupt(digitalPinToInterrupt(TOUCH_INT), TouchISR, FALLING);
  
}

int pinState = 0, lastPinState = 0;
uint16_t t_x = 0, t_y = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //Serial.println(pinState);
  
  if(tft.getTouch(&t_x, &t_y)){
    Serial.print(t_x);
    Serial.print(", ");
    Serial.print(t_y);
    Serial.println();
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
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
  
}

/*
// not working presently, don't forget IRAM declaration
void TouchISR(){
  digitalWrite(LED_BUILTIN, HIGH);
}
*/


//==================================================================

#ifdef CAL_TS
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
