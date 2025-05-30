/*
 * Tiny4kOLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x32 displays
 *
 * Based on ssd1306xled, re-written and extended by Stephen Denne
 * from 2017-04-25 at https://github.com/datacute/Tiny4kOLED
 *
 */

// Choose your I2C implementation before including Tiny4kOLED.h
// The default is selected is Wire.h

// To use the Wire library:
//#include <Wire.h>

// To use the Adafruit's TinyWireM library:
//#include <TinyWireM.h>

// To use the TinyI2C library from https://github.com/technoblogy/tiny-i2c
//#include <TinyI2CMaster.h>

// The blue OLED screen requires a long initialization on power on.
// The code to wait for it to be ready uses 20 bytes of program storage space
// If you are using a white OLED, this can be reclaimed by uncommenting
// the following line (before including Tiny4kOLED.h):
//#define TINY4KOLED_QUICK_BEGIN

#include <Tiny4kOLED.h>
#include "font16x16cn.h"
// ============================================================================

void setup() {
  // put your setup code here, to run once:

  oled.begin();
  // Two rotations are supported, 
  // The begin() method sets the rotation to 1.
  //oled.setRotation(0);
  oled.clear();
  oled.setFont(&TinyOLED4kfont16x16cn);
  oled.setCursor(10, 1);
  oled.print(F("01234"));
  oled.on();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}
