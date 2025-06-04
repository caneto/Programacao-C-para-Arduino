#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Pushbutton.h>

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
int reading;
int lastReading;
#define CALIBRATION_FACTOR -476.592

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 4
Pushbutton button(BUTTON_PIN);

void displayWeight(int weight){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Weight:");
  display.display();
  display.setCursor(0, 30);
  display.setTextSize(2);
  display.print(weight);
  display.print(" ");
  display.print("g");
  display.display();  
}

void setup() {
  Serial.begin(57200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();               
}

void loop() {
  
  if (button.getSingleDebouncedPress()){
    Serial.print("tare...");
    scale.tare();
  }
  
  if (scale.wait_ready_timeout(200)) {
    reading = round(scale.get_units());
    Serial.print("Weight: ");
    Serial.println(reading);
    if (reading != lastReading){
      displayWeight(reading); 
    }
    lastReading = reading;
  }
  else {
    Serial.println("HX711 not found.");
  }
}

